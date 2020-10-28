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


#include "paddle_lite_machine_predictor.h"
#include "../common_log.h"
#include "../mml_inference_api.h"
#include "../paddle_lite_header/paddle_api.h"

mml_framework::ErrorCode fillLiteConfigWithMMLConfig_Business(paddle::lite_api::MobileConfig &aconfig,
                                                         mml_framework::MMLConfig::PaddleLiteConfig &liteconfig,
                                                                     const mml_framework::MMLConfig &config) {
    mml_framework::ErrorCode error_code = mml_framework::ErrorCode::SUCCESS;
    switch (config.machine_config.paddle_lite_config.model_type) {
        case mml_framework::MMLConfig::PaddleLiteConfig::LITE_MODEL_FROM_DIR: {
            aconfig.set_model_dir(config.modelUrl);
            break;
        }
        case mml_framework::MMLConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE: {
            std::string model_file = std::string(config.machine_config.paddle_lite_config.model.model_from_file.data,
                                                 config.machine_config.paddle_lite_config.model.model_from_file.size);
            aconfig.set_model_from_file(model_file);
            break;
        }
        case mml_framework::MMLConfig::PaddleLiteConfig::LITE_MODEL_FROM_BUFFER: {
            std::string model_buffer =
            std::string(config.machine_config.paddle_lite_config.model.model_from_buffer.data,
                        config.machine_config.paddle_lite_config.model.model_from_buffer.size);
            aconfig.set_model_from_buffer(model_buffer);
            break;
        }
        case mml_framework::MMLConfig::PaddleLiteConfig::LITE_MODEL_FROM_MODELBUFFER: {
            aconfig.set_model_buffer(config.machine_config.paddle_lite_config.model.model_buffer.model_buffer,
                                     config.machine_config.paddle_lite_config.model.model_buffer.model_buffer_size,
                                     config.machine_config.paddle_lite_config.model.model_buffer.param_buffer,
                                     config.machine_config.paddle_lite_config.model.model_buffer.param_buffer_size);
            break;
        }
        default: {
            error_code = mml_framework::ErrorCode::LOAD_ERR_OTHER;
            break;
        }
    }
    return error_code;
}

int PaddleLiteMachinePredictor::load(const mml_framework::MMLConfig &config) { FUNC_BEGIN_WITHTIME
    
    int error_code = mml_framework::ErrorCode::SUCCESS;
    
    paddle::lite_api::MobileConfig aconfig;
    mml_framework::MMLConfig::PaddleLiteConfig liteconfig = config.machine_config.paddle_lite_config;
  
    error_code = fillLiteConfigWithMMLConfig_Business(aconfig, liteconfig, config);
    
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
            error_code = mml_framework::ErrorCode::LOAD_ERR_OTHER;
            break;
    }

    if (mml_framework::ErrorCode::SUCCESS == error_code) {
        CALL_BEGIN_WITHTIME("paddle::lite_api::CreatePaddlePredictor")
        this->realPredictorPtr =
        paddle::lite_api::CreatePaddlePredictor<paddle::lite_api::MobileConfig>(aconfig);
        CALL_END_WITHTIME("paddle::lite_api::CreatePaddlePredictor")
    }

    if (nullptr == this->realPredictorPtr) {
        error_code = mml_framework::ErrorCode::LOAD_ERR_OTHER;
    }
    
    
    FUNC_END_WITHTIME
    return error_code;
}

int PaddleLiteMachinePredictor::predict(){ FUNC_BEGIN_WITHTIME
    
    LOGI("enter PaddleLiteMachinePredictor::predict\n");

    if (realPredictorPtr == nullptr) {
        LOGI("realPredictor is nullptr, did you call load() first?");
        return mml_framework::ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    CALL_BEGIN_WITHTIME("litePredictor->Run")
    litePredictor->Run();
    CALL_END_WITHTIME("litePredictor->Run")
    
    FUNC_END_WITHTIME
    return 0;
}

int PaddleLiteMachinePredictor::predict(mml_framework::MMLData &modelInputData, mml_framework::MMLData *modelOutputData) {

    LOGI("enter PaddleLiteMachinePredictor::predict");

    if (realPredictorPtr == nullptr) {
        LOGI("realPredictor is nullptr, did you call load() first?");
        return mml_framework::ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    (*litePredictor).Run();

    return mml_framework::ErrorCode::SUCCESS;
}

std::unique_ptr<mml_framework::MMLData> PaddleLiteMachinePredictor::getInputData(int i){
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
    
    std::unique_ptr<paddle::lite_api::Tensor> input_tensor((*litePredictor).GetInput(i));
    paddle::lite_api::Tensor *tensor = input_tensor.release();
    std::unique_ptr<mml_framework::MMLData> data(new mml_framework::MMLData());

    mml_framework::MMLTensor *mmlTensor = new mml_framework::MMLTensor();
    data->mmlTensor = mmlTensor;
    data->mmlTensor->tensor = tensor;
    return data;
}
   
std::unique_ptr<const mml_framework::MMLData> PaddleLiteMachinePredictor::getOutputData(int i){ FUNC_BEGIN_WITHTIME
    
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
       
    CALL_BEGIN_WITHTIME("std::unique_ptr<const paddle::lite_api::Tensor> GetOutput")
    std::unique_ptr< const paddle::lite_api::Tensor> output_tensor((*litePredictor).GetOutput(i));
    CALL_END_WITHTIME("std::unique_ptr<const paddle::lite_api::Tensor> GetOutput")
    
    const paddle::lite_api::Tensor *tensor = output_tensor.release();
    std::unique_ptr<mml_framework::MMLData> data(new mml_framework::MMLData());
    
    mml_framework::MMLTensor *mmlTensor = new mml_framework::MMLTensor();
    data->mmlTensor = mmlTensor;
    data->mmlTensor->tensor = (void *)tensor;
    
    mml_framework::shape_t shape = mmlTensor->shape();
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

std::unique_ptr<mml_framework::MMLData> PaddleLiteMachinePredictor::getInputByName(const std::string& name) {
    paddle::lite_api::PaddlePredictor *litePredictor = static_cast<paddle::lite_api::PaddlePredictor *>( this->realPredictorPtr.get());
       
    std::unique_ptr<paddle::lite_api::Tensor> input_tensor((*litePredictor).GetInputByName(name));
    paddle::lite_api::Tensor *tensor = input_tensor.release();
    std::unique_ptr<mml_framework::MMLData> data(new mml_framework::MMLData());
    
    mml_framework::MMLTensor *mmlTensor = new mml_framework::MMLTensor();
    data->mmlTensor = mmlTensor;
    data->mmlTensor->tensor = (void *)tensor;
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
