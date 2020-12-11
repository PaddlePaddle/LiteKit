/**
Copyright © 2020 Baidu, Inc. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * LiteKit framework
 */

#include <vector>
#include "litekit_inference_api.h"
#include "common_log.h"
#include <stdlib.h>
#include <iostream>
#include "paddle_lite_header/paddle_api.h"
#include "litekit_convertor.h"
#include "paddle_lite/paddle_lite_machine_predictor.h"
#ifdef BACKEND_PADDLE_GPU
#include "paddle_gpu/paddle_gpu_machine_predictor.h"
#endif

using namespace litekit_framework;

void LiteKitMachineService::setInterceptProcessor(LiteKitDataProcessor *processorImpl) {
    mProcessorImpl = processorImpl;
}

int litekit_framework::LiteKitMachineService::load(const LiteKitConfig &config) { FUNC_BEGIN_WITHTIME
    
    if (machineHandle != nullptr) {
        LOGI("LiteKitMachineService was already loaded and not released");
        // release and reload()
        release();
    }

    if (config.machine_type == litekit_framework::LiteKitConfig::MachineType::PaddleLite){
        this->litekitMachineType = litekit_framework::LiteKitMachineType::PADDLE_LITE;
        PaddleLiteMachinePredictor *paddleLiteHandler = new PaddleLiteMachinePredictor();
        int errorCode = paddleLiteHandler->load(config);
        if (errorCode == ErrorCode::SUCCESS) {
            machineHandle = paddleLiteHandler;
        } else {
            delete paddleLiteHandler;
        }

        FUNC_END_WITHTIME
        return errorCode;
    }else if (config.machine_type == litekit_framework::LiteKitConfig::MachineType::PaddleiOSGPU){
#ifdef BACKEND_PADDLE_GPU
        this->litekitMachineType = litekit_framework::LiteKitMachineType::PADDLE_iOSGPU;
        PaddleGPUMachinePredictor *paddleGPUHandler = new PaddleGPUMachinePredictor();
        int errorCode = paddleGPUHandler->load(config);
        if (errorCode == ErrorCode::SUCCESS) {
            machineHandle = paddleGPUHandler;
        } else {
            delete paddleGPUHandler;
        }
        return errorCode;
#else
        return ErrorCode:: RUN_ERR_MACHINE_TYPE;
#endif
    }else{
        return ErrorCode:: RUN_ERR_MACHINE_TYPE;
    }
}
int litekit_framework::LiteKitMachineService::run(){ FUNC_BEGIN_WITHTIME
    
    int predictErrorCode = predict();
    
    FUNC_END_WITHTIME
    return predictErrorCode;
}
int litekit_framework::LiteKitMachineService::run(LiteKitData& inputData, LiteKitData *outputData) {

    if (machineHandle == nullptr) {
        LOGI("machineHandle is nullptr, did you call load() first?");
        return ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }

    if (mProcessorImpl != nullptr) {
        LOGI("preProcess begin");
        LiteKitData modelInputData = LiteKitData();
        LiteKitData modelOutputData = LiteKitData();

        int preProcessErrorCode = mProcessorImpl->preProcess(inputData, &modelInputData);
        if (preProcessErrorCode != ErrorCode::SUCCESS) {
            LOGI("preProcess error : %d", preProcessErrorCode);
            return preProcessErrorCode;
        }

        LOGI("predict begin");
        int predictErrorCode = predict(modelInputData, &modelOutputData);

        if (predictErrorCode != ErrorCode::SUCCESS) {
            LOGI("predict error : %d", predictErrorCode);
            return predictErrorCode;
        }

        LOGI("postProcess begin");
        int postProcessErrorCode = mProcessorImpl->postProcess(modelOutputData, outputData);
        if (postProcessErrorCode != ErrorCode::SUCCESS) {
            LOGI("postProcess error : %d", postProcessErrorCode);
            return postProcessErrorCode;
        }
        return ErrorCode::SUCCESS;
    } else {

        LOGI("mProcessorImpl null");
        int predictErrorCode = predict(inputData, outputData);

        if (predictErrorCode != ErrorCode::SUCCESS) {
            LOGI("predict error : %d", predictErrorCode);
        }
        return predictErrorCode;
    }
}

int litekit_framework::LiteKitMachineService::predict(LiteKitData &modelInputData, LiteKitData *modelOutputData) {
    LOGI("LiteKitMachineService::predict begin\n");
    BaseMachinePredictor* baseMachinePredictor = static_cast<BaseMachinePredictor*>(machineHandle);
    return baseMachinePredictor->predict(modelInputData, modelOutputData);
}

int litekit_framework::LiteKitMachineService::predict() {
    LOGI("LiteKitMachineService::predict begin\n");
    BaseMachinePredictor* baseMachinePredictor = static_cast<BaseMachinePredictor*>(machineHandle);
    return baseMachinePredictor->predict();
}

std::unique_ptr<litekit_framework::LiteKitData> LiteKitMachineService::getInputData(int i){
    std::unique_ptr<litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());
    if (this->litekitMachineType == litekit_framework::LiteKitMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data.reset(litePredictor->getInputData(i).release());
    }
    return data;
}

std::unique_ptr<const litekit_framework::LiteKitData> LiteKitMachineService::getOutputData(int i){ FUNC_BEGIN_WITHTIME
    std::unique_ptr<const litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());
    if (this->litekitMachineType == litekit_framework::LiteKitMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data.reset((litePredictor->getOutputData(i)).release());
    }
    FUNC_END_WITHTIME
    return data;
}

std::vector<std::string> LiteKitMachineService::getInputNames() {
    std::vector<std::string> inputNames = {};
    if (this->litekitMachineType == litekit_framework::LiteKitMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        inputNames = litePredictor->getInputNames();
    }
    return inputNames;
}

std::vector<std::string> LiteKitMachineService::getOutputNames() {
    std::vector<std::string> outputNames = {};
    if (this->litekitMachineType == litekit_framework::LiteKitMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        outputNames = litePredictor->getOutputNames();
    }
    return outputNames;
}

std::unique_ptr<litekit_framework::LiteKitData> LiteKitMachineService::getInputByName(const std::string& name) {
    std::unique_ptr<litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());
    if (this->litekitMachineType == litekit_framework::LiteKitMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data = litePredictor->getInputByName(name);
    }
    return data;
}

LiteKitMachineService::~LiteKitMachineService() {
    LOGI("destructor of LiteKitMachineService, autoRelease %d\n", autoRelease);
    if (autoRelease) {
        release();
    }
}


void LiteKitMachineService::release() {

    if (mProcessorImpl != nullptr) {
        delete mProcessorImpl;
        mProcessorImpl = nullptr;
    }

    if (machineHandle != nullptr) {
        BaseMachinePredictor* baseMachinePredictor = static_cast<BaseMachinePredictor*>(machineHandle);
        delete baseMachinePredictor;
        machineHandle = nullptr;
    }
}

void LiteKitTensor::Resize(const shape_t &shape){
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    tensor->Resize(shape);
};

void LiteKitTensor::SetLoD(const lod_t& lod){
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    tensor->SetLoD(lod);
};

///implement const T* data() const;
template <>
const int *LiteKitTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int>();
}

template <>
const float *LiteKitTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<float>();
}

template <>
const int8_t *LiteKitTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int8_t>();
}
template <>
const uint8_t *LiteKitTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<uint8_t>();
}
template <>
const int64_t *LiteKitTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int64_t>();
}

///T* mutable_data(LiteKitTargetType type = LiteKitTargetType::kHost) const;
template <>
int *LiteKitTensor::mutable_data(LiteKitTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int>();
}

template <>
float *LiteKitTensor::mutable_data(LiteKitTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<float>();
}

template <>
int8_t *LiteKitTensor::mutable_data(LiteKitTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int8_t>();
}

template <>
uint8_t *LiteKitTensor::mutable_data(LiteKitTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<uint8_t>();
}

template <>
int64_t *LiteKitTensor::mutable_data(LiteKitTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int64_t>();
}

shape_t LiteKitTensor::shape() const{
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->shape();
}

lod_t LiteKitTensor::lod() const{
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->lod();
}

void LiteKitTensor::release() {
    if (nullptr != tensor) {
        delete tensor;
    }
}

LiteKitTensor::~LiteKitTensor() {
    LOGI("destructor of LiteKitTensor, autoRelease %d\n", autoRelease);
    if (autoRelease) {
        release();
    }
}

void LiteKitData::release() {
    if (rawData != nullptr) {
        delete[] rawData;
        rawData = nullptr;
        dataLength = 0;
    }
    if (nullptr != litekitTensor) {
        delete litekitTensor;
        litekitTensor = nullptr;
    }
}

LiteKitData::~LiteKitData() {
    LOGI("destructor of LiteKitData, autoRelease %d", autoRelease);
    if (autoRelease) {
        release();
    }
}


std::shared_ptr<LiteKitMachineService> litekit_framework::CreateLiteKitMachineService (LiteKitConfig& config) {
  std::shared_ptr<LiteKitMachineService> service(new litekit_framework::LiteKitMachineService());
  service->load(config);
  return service;
}
