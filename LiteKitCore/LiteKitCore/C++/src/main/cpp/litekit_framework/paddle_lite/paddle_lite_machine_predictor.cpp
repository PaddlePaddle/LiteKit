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

//
//  paddle_lite_machine_predictor.cpp
//  LiteKit
//

#include "paddle_lite_machine_predictor.h"
#include "../common_log.h"
#include "../litekit_inference_api.h"
#include "../paddle_lite_header/paddle_api.h"

litekit_framework::ErrorCode fillLiteConfigWithLiteKitConfig_Business(paddle::lite_api::MobileConfig &aconfig,
                                                         litekit_framework::LiteKitConfig::PaddleLiteConfig &liteconfig,
                                                                     const litekit_framework::LiteKitConfig &config) {
    litekit_framework::ErrorCode error_code = litekit_framework::ErrorCode::SUCCESS;
    switch (config.machine_config.paddle_lite_config.model_type) {
        case litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_DIR: {
            aconfig.set_model_dir(config.modelUrl);
            break;
        }
        case litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE: {
            std::string model_file = std::string(config.machine_config.paddle_lite_config.model.model_from_file.data,
                                                 config.machine_config.paddle_lite_config.model.model_from_file.size);
            aconfig.set_model_from_file(model_file);
            break;
        }
        case litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_BUFFER: {
            std::string model_buffer =
            std::string(config.machine_config.paddle_lite_config.model.model_from_buffer.data,
                        config.machine_config.paddle_lite_config.model.model_from_buffer.size);
            aconfig.set_model_from_buffer(model_buffer);
            break;
        }
        case litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_MODELBUFFER: {
            aconfig.set_model_buffer(config.machine_config.paddle_lite_config.model.model_buffer.model_buffer,
                                     config.machine_config.paddle_lite_config.model.model_buffer.model_buffer_size,
                                     config.machine_config.paddle_lite_config.model.model_buffer.param_buffer,
                                     config.machine_config.paddle_lite_config.model.model_buffer.param_buffer_size);
            break;
        }
        default: {
            error_code = litekit_framework::ErrorCode::LOAD_ERR_OTHER;
            break;
        }
    }
    return error_code;
}

int PaddleLiteMachinePredictor::load(const litekit_framework::LiteKitConfig &config) { FUNC_BEGIN_WITHTIME
    
    int error_code = litekit_framework::ErrorCode::SUCCESS;
    
    paddle::lite_api::MobileConfig aconfig;
    litekit_framework::LiteKitConfig::PaddleLiteConfig liteconfig = config.machine_config.paddle_lite_config;
  
    error_code = fillLiteConfigWithLiteKitConfig_Business(aconfig, liteconfig, config);
    
    aconfig.set_threads(liteconfig.threads);
    
    int currentPowerMode = liteconfig.powermode;
    switch (currentPowerMode) {
        case paddle::lite_api::LITE_POWER_HIGH:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_HIGH);
            break;
        case paddle::lite_api::LITE_POWER_LOW:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_LOW);
            break;
        case paddle::lite_api::LITE_POWER_FULL:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_FULL);
            break;
        case paddle::lite_api::LITE_POWER_NO_BIND:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_NO_BIND);
            break;
        case paddle::lite_api::LITE_POWER_RAND_HIGH:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_RAND_HIGH);
            break;
        case paddle::lite_api::LITE_POWER_RAND_LOW:
            aconfig.set_power_mode(paddle::lite_api::LITE_POWER_RAND_LOW);
            break;
        default:
            error_code = litekit_framework::ErrorCode::LOAD_ERR_OTHER;
            break;
    }

    if (litekit_framework::ErrorCode::SUCCESS == error_code) {
        CALL_BEGIN_WITHTIME("paddle::lite_api::CreatePaddlePredictor")
        this->realPredictorPtr =
        paddle::lite_api::CreatePaddlePredictor<paddle::lite_api::MobileConfig>(aconfig);
        CALL_END_WITHTIME("paddle::lite_api::CreatePaddlePredictor")
    }

    if (nullptr == this->realPredictorPtr) {
        error_code = litekit_framework::ErrorCode::LOAD_ERR_OTHER;
    }
    
    
    FUNC_END_WITHTIME
    return error_code;
}

int PaddleLiteMachinePredictor::predict(){ FUNC_BEGIN_WITHTIME
    
    LOGI("enter PaddleLiteMachinePredictor::predict\n");

    if (realPredictorPtr == nullptr) {
        LOGI("realPredictor is nullptr, did you call load() first?");
        return litekit_framework::ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    CALL_BEGIN_WITHTIME("litePredictor->Run")
    litePredictor->Run();
    CALL_END_WITHTIME("litePredictor->Run")
    
    FUNC_END_WITHTIME
    return 0;
}

int PaddleLiteMachinePredictor::predict(litekit_framework::LiteKitData &modelInputData, litekit_framework::LiteKitData *modelOutputData) {

    LOGI("enter PaddleLiteMachinePredictor::predict");

    if (realPredictorPtr == nullptr) {
        LOGI("realPredictor is nullptr, did you call load() first?");
        return litekit_framework::ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    (*litePredictor).Run();

    return litekit_framework::ErrorCode::SUCCESS;
}

std::unique_ptr<litekit_framework::LiteKitData> PaddleLiteMachinePredictor::getInputData(int i){
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    std::unique_ptr<paddle::lite_api::Tensor> input_tensor((*litePredictor).GetInput(i));
    paddle::lite_api::Tensor *tensor = input_tensor.release();
    std::unique_ptr<litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());

    litekit_framework::LiteKitTensor *litekitTensor = new litekit_framework::LiteKitTensor();
    data->litekitTensor = litekitTensor;
    data->litekitTensor->tensor = tensor;
    return data;
}
   
std::unique_ptr<const litekit_framework::LiteKitData> PaddleLiteMachinePredictor::getOutputData(int i){ FUNC_BEGIN_WITHTIME
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
       
    CALL_BEGIN_WITHTIME("std::unique_ptr<const paddle::lite_api::Tensor> GetOutput")
    std::unique_ptr< const paddle::lite_api::Tensor> output_tensor((*litePredictor).GetOutput(i));
    CALL_END_WITHTIME("std::unique_ptr<const paddle::lite_api::Tensor> GetOutput")
    
    const paddle::lite_api::Tensor *tensor = output_tensor.release();
    std::unique_ptr<litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());
    
    litekit_framework::LiteKitTensor *litekitTensor = new litekit_framework::LiteKitTensor();
    data->litekitTensor = litekitTensor;
    data->litekitTensor->tensor = (void *)tensor;
    
    litekit_framework::shape_t shape = litekitTensor->shape();
    if (shape.size() > 0) {
        int64_t res = 1;
        for (auto i : shape) { res *= i; }
        data->dataLength = res;
    }
    
    FUNC_END_WITHTIME
    return data;
}

std::vector<std::string> PaddleLiteMachinePredictor::getInputNames() {
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    return litePredictor->GetInputNames();
}

std::vector<std::string> PaddleLiteMachinePredictor::getOutputNames() {
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    return litePredictor->GetOutputNames();
}

std::unique_ptr<litekit_framework::LiteKitData> PaddleLiteMachinePredictor::getInputByName(const std::string& name) {
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
       
    std::unique_ptr<paddle::lite_api::Tensor> input_tensor((*litePredictor).GetInputByName(name));
    paddle::lite_api::Tensor *tensor = input_tensor.release();
    std::unique_ptr<litekit_framework::LiteKitData> data(new litekit_framework::LiteKitData());
    
    litekit_framework::LiteKitTensor *litekitTensor = new litekit_framework::LiteKitTensor();
    data->litekitTensor = litekitTensor;
    data->litekitTensor->tensor = (void *)tensor;
    return data;
}

PaddleLiteMachinePredictor::~PaddleLiteMachinePredictor() {

    LOGI("destructor of PaddleLiteMachinePredictor\n");
    if (realPredictorPtr!=nullptr) {
        
    }
    if (realPredictorPtr != nullptr) {
        realPredictorPtr.reset();
    }
}
