// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * MML framework
 */

#include <vector>
#include "mml_inference_api.h"
#include "common_log.h"
#include <stdlib.h>
#include <iostream>
#include "paddle_lite_header/paddle_api.h"
#include "mml_convertor.h"
#include "paddle_lite/paddle_lite_machine_predictor.h"
#ifdef BACKEND_PADDLE_GPU
#include "paddle_gpu/paddle_gpu_machine_predictor.h"
#endif

using namespace mml_framework;

void MMLMachineService::setInterceptProcessor(MMLDataProcessor *processorImpl) {
    mProcessorImpl = processorImpl;
}

int mml_framework::MMLMachineService::load(const MMLConfig &config) { FUNC_BEGIN_WITHTIME
    
    if (machineHandle != nullptr) {
        LOGI("MMLMachineService was already loaded and not released");
        // 释放并重新load()
        release();
    }

    if (config.machine_type == mml_framework::MMLConfig::MachineType::PaddleLite){
        this->mmlMachineType = mml_framework::MMLMachineType::PADDLE_LITE;
        PaddleLiteMachinePredictor *paddleLiteHandler = new PaddleLiteMachinePredictor();
        int errorCode = paddleLiteHandler->load(config);
        if (errorCode == ErrorCode::SUCCESS) {
            machineHandle = paddleLiteHandler;
        } else {
            delete paddleLiteHandler;
        }

        FUNC_END_WITHTIME
        return errorCode;
    }else if (config.machine_type == mml_framework::MMLConfig::MachineType::PaddleiOSGPU){
#ifdef BACKEND_PADDLE_GPU
        this->mmlMachineType = mml_framework::MMLMachineType::PADDLE_iOSGPU;
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
int mml_framework::MMLMachineService::run(){ FUNC_BEGIN_WITHTIME
    
    int predictErrorCode = predict();
    
    FUNC_END_WITHTIME
    return predictErrorCode;
}
int mml_framework::MMLMachineService::run(MMLData& inputData, MMLData *outputData) {

    if (machineHandle == nullptr) {
        LOGI("machineHandle is nullptr, did you call load() first?");
        return ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }

    if (mProcessorImpl != nullptr) {
        LOGI("preProcess begin");
        MMLData modelInputData = MMLData();
        MMLData modelOutputData = MMLData();

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

int mml_framework::MMLMachineService::predict(MMLData &modelInputData, MMLData *modelOutputData) {
    LOGI("MMLMachineService::predict begin\n");
    BaseMachinePredictor* baseMachinePredictor = static_cast<BaseMachinePredictor*>(machineHandle);
    return baseMachinePredictor->predict(modelInputData, modelOutputData);
}

int mml_framework::MMLMachineService::predict() {
    LOGI("MMLMachineService::predict begin\n");
    BaseMachinePredictor* baseMachinePredictor = static_cast<BaseMachinePredictor*>(machineHandle);
    return baseMachinePredictor->predict();
}

std::unique_ptr<mml_framework::MMLData> MMLMachineService::getInputData(int i){
    std::unique_ptr<mml_framework::MMLData> data(new mml_framework::MMLData());
    if (this->mmlMachineType == mml_framework::MMLMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data.reset(litePredictor->getInputData(i).release());
    }
    return data;
}

std::unique_ptr<const mml_framework::MMLData> MMLMachineService::getOutputData(int i){ FUNC_BEGIN_WITHTIME
    std::unique_ptr<const mml_framework::MMLData> data(new mml_framework::MMLData());
    if (this->mmlMachineType == mml_framework::MMLMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data.reset((litePredictor->getOutputData(i)).release());
    }
    FUNC_END_WITHTIME
    return data;
}

std::vector<std::string> MMLMachineService::getInputNames() {
    std::vector<std::string> inputNames = {};
    if (this->mmlMachineType == mml_framework::MMLMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        inputNames = litePredictor->getInputNames();
    }
    return inputNames;
}

std::vector<std::string> MMLMachineService::getOutputNames() {
    std::vector<std::string> outputNames = {};
    if (this->mmlMachineType == mml_framework::MMLMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        outputNames = litePredictor->getOutputNames();
    }
    return outputNames;
}

std::unique_ptr<mml_framework::MMLData> MMLMachineService::getInputByName(const std::string& name) {
    std::unique_ptr<mml_framework::MMLData> data(new mml_framework::MMLData());
    if (this->mmlMachineType == mml_framework::MMLMachineType::PADDLE_LITE){
        PaddleLiteMachinePredictor *litePredictor = static_cast<PaddleLiteMachinePredictor *>(this->machineHandle);
        data = litePredictor->getInputByName(name);
    }
    return data;
}

MMLMachineService::~MMLMachineService() {
    LOGI("destructor of MMLMachineService, autoRelease %d\n", autoRelease);
    if (autoRelease) {
        release();
    }
}


void MMLMachineService::release() {

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

void MMLTensor::Resize(const shape_t &shape){
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    tensor->Resize(shape);
};

void MMLTensor::SetLoD(const lod_t& lod){
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    tensor->SetLoD(lod);
};

///implement const T* data() const;
template <>
const int *MMLTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int>();
}

template <>
const float *MMLTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<float>();
}

template <>
const int8_t *MMLTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int8_t>();
}
template <>
const uint8_t *MMLTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<uint8_t>();
}
template <>
const int64_t *MMLTensor::data() const {
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->data<int64_t>();
}

///T* mutable_data(MMLTargetType type = MMLTargetType::kHost) const;
template <>
int *MMLTensor::mutable_data(MMLTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int>();
}

template <>
float *MMLTensor::mutable_data(MMLTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<float>();
}

template <>
int8_t *MMLTensor::mutable_data(MMLTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int8_t>();
}

template <>
uint8_t *MMLTensor::mutable_data(MMLTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<uint8_t>();
}

template <>
int64_t *MMLTensor::mutable_data(MMLTargetType type) const {
    paddle::lite_api::Tensor *tensor = static_cast<paddle::lite_api::Tensor *>(this->tensor);
    return tensor->mutable_data<int64_t>();
}

shape_t MMLTensor::shape() const{
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->shape();
}

lod_t MMLTensor::lod() const{
    const paddle::lite_api::Tensor *output_tensor = static_cast<const paddle::lite_api::Tensor *>(this->tensor);
    return output_tensor->lod();
}

void MMLTensor::release() {
    if (nullptr != tensor) {
        delete tensor;
    }
}

MMLTensor::~MMLTensor() {
    LOGI("destructor of MMLTensor, autoRelease %d\n", autoRelease);
    if (autoRelease) {
        release();
    }
}

void MMLData::release() {
    if (rawData != nullptr) {
        delete[] rawData;
        rawData = nullptr;
        dataLength = 0;
    }
    if (nullptr != mmlTensor) {
        delete mmlTensor;
        mmlTensor = nullptr;
    }
}

MMLData::~MMLData() {
    LOGI("destructor of MMLData, autoRelease %d", autoRelease);
    if (autoRelease) {
        release();
    }
}


std::shared_ptr<MMLMachineService> mml_framework::CreateMMLMachineService (MMLConfig& config) {
  std::shared_ptr<MMLMachineService> service(new mml_framework::MMLMachineService());
  service->load(config);
  return service;
}
