
#include "paddle_gpu_machine_predictor.h"
#include "../mml_inference_api.h"
#include <stdlib.h>
#include "../common_log.h"
#include "../mml_convertor.h"
#include "../paddle_gpu_header/paddle_mobile_gpu_interface.h"

int PaddleGPUMachinePredictor::load(const mml_framework::MMLConfig &config) {
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

int PaddleGPUMachinePredictor::predict(MMLData &modelInputData, MMLData *modelOutputData) {
    
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
            modelOutputData->rawDataShape = mml_framework::MMLData::RawDataShape(output.shape[0], output.shape[1], output.shape[2] ,output.shape[3]);
        }
    }
    return ErrorCode::SUCCESS;
}

std::unique_ptr<mml_framework::MMLData> PaddleGPUMachinePredictor::getInputData(int i) {
    return 0;
}

std::unique_ptr<const mml_framework::MMLData> PaddleGPUMachinePredictor::getOutputData(int i) {
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

std::unique_ptr<mml_framework::MMLData> PaddleGPUMachinePredictor::getInputByName(const std::string& name) {
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
