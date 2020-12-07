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

#include "paddle_gpu_machine_predictor.h"
#include "../litekit_inference_api.h"
#include <stdlib.h>
#include "../common_log.h"
#include "../litekit_convertor.h"
#include "../paddle_gpu_header/paddle_mobile_gpu_interface.h"

int PaddleGPUMachinePredictor::load(const litekit_framework::LiteKitConfig &config) {
    LOGI("enter PaddleGPUMachinePredictor::load");
    
    paddle_moible_gpu::PaddleMobileGPUModelConfig modelConfig = {};
    modelConfig.model_path = config.modelUrl + "/" + config.model_file_name;
    modelConfig.param_path = config.modelUrl + "/" + config.param_file_name;
    paddle_moible_gpu::PaddleMobileGPUPredictor *predictor = new paddle_moible_gpu::PaddleMobileGPUPredictor();
    predictor->init(modelConfig);
    predictor->load();
    this->realPredictor = predictor;
    
    return ErrorCode::SUCCESS;
}

int PaddleGPUMachinePredictor::predict(LiteKitData &modelInputData, LiteKitData *modelOutputData) {
    
    LOGI("enter PaddleLiteMachinePredictor::predict");

    if (realPredictor == nullptr) {
        LOGI("realPredictor is nullptr, did you call load() first?");
        return ErrorCode::RUN_ERR_MACHINE_HANDLE;
    }

    if (modelInputData.rawData == nullptr || modelInputData.dataLength <= 0) {
        LOGI("param error : dataLength");
        return ErrorCode::ERR_PARAM;
    }

    paddle_moible_gpu::PaddleMobileGPUData input = {};
    input.data = modelInputData.rawData;
    input.length = modelInputData.dataLength;
    input.shape = {
        modelInputData.rawDataShape.n,
        modelInputData.rawDataShape.c,
        modelInputData.rawDataShape.h,
        modelInputData.rawDataShape.w
    };
    std::vector<paddle_moible_gpu::PaddleMobileGPUData> outputs;
    paddle_moible_gpu::PaddleMobileGPUPredictor *paddle_gpu = static_cast<paddle_moible_gpu::PaddleMobileGPUPredictor *>(realPredictor);
    if (paddle_gpu != NULL) {
        bool is_success = paddle_gpu->predict(input, &outputs);
        if (!is_success) {
            return ErrorCode::RUN_ERR_PREDICT;
        }
        if (outputs.size() > 0) {
            paddle_moible_gpu::PaddleMobileGPUData output = outputs[0];
            modelOutputData->rawData = output.data;
            modelOutputData->dataLength = output.length;
            modelOutputData->rawDataShape = litekit_framework::LiteKitData::RawDataShape(output.shape[0], output.shape[1], output.shape[2] ,output.shape[3]);
        }
    }
    return ErrorCode::SUCCESS;
}

std::unique_ptr<litekit_framework::LiteKitData> PaddleGPUMachinePredictor::getInputData(int i) {
    return 0;
}

std::unique_ptr<const litekit_framework::LiteKitData> PaddleGPUMachinePredictor::getOutputData(int i) {
    return 0;
}

int PaddleGPUMachinePredictor::predict(){
    return -1;
}

std::vector<std::string> PaddleGPUMachinePredictor::getInputNames() {
    return {};
}

std::vector<std::string> PaddleGPUMachinePredictor::getOutputNames() {
    return {};
}

std::unique_ptr<litekit_framework::LiteKitData> PaddleGPUMachinePredictor::getInputByName(const std::string& name) {
    return 0;
}

PaddleGPUMachinePredictor::~PaddleGPUMachinePredictor() {
    LOGI("destructor of PaddleGPUMachinePredictor\n");
    if (realPredictor != nullptr) {
        paddle_moible_gpu::PaddleMobileGPUPredictor *paddle_gpu = static_cast<paddle_moible_gpu::PaddleMobileGPUPredictor *>(realPredictor);
        if (paddle_gpu != NULL) {
            delete paddle_gpu;
            realPredictor = nullptr;
        }
    }
}
