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

/**
 * BaseMachinePredictor封装，不同Backend实现（如PaddleLite）应当继承自BaseMachinePredictor
 */

#ifndef LIB_AI_BASE_MACHINE_PREDICTOR_H
#define LIB_AI_BASE_MACHINE_PREDICTOR_H

#include "mml_inference_api.h"

class BaseMachinePredictor {
public:
    // 真实的BackendPredictor指针（如PaddleLite）
    void *realPredictor;
    // backend的share pointer
    std::shared_ptr<void> realPredictorPtr;

    /**
     * 根据config，创建realPredictor，并加载模型
     *
     * @param config
     * @return
     */
    virtual int load(const mml_framework::MMLConfig &config) = 0;

    /**
     * 预测函数
     * @param inputData
     * @param outputData
     * @return
     */
    virtual int predict(mml_framework::MMLData &inputData, mml_framework::MMLData *outputData) = 0;
    /**
    * 预测函数 适用于paddle-lite backend
    * @return i 0成功, 其他错误见mml_framework ErrorCode
    */
    virtual int predict() = 0;
    /**
       * 申请第i个input的tensor空间
       * @param i
       * @return MMLData
       */
    virtual std::unique_ptr<mml_framework::MMLData> getInputData(int i) = 0;
    /**
       * 获取多输出中的第i个output
       * @param i
       * @return MMLData
       */
    virtual std::unique_ptr<const mml_framework::MMLData> getOutputData(int i) = 0;
    /**
     * 获取input的names
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getInputNames() = 0 ;
    /**
     * 获取output的names
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getOutputNames() = 0 ;
    /**
     * 获取name对应的input
     * @param name input的name
     * @return MMLData
     */
    virtual std::unique_ptr<mml_framework::MMLData> getInputByName(const std::string& name) = 0 ;
    
    /**
     * 析构函数。会delete realPredictor 并置空。
     */
    virtual ~BaseMachinePredictor() {};
};


#endif //LIB_AI_BASE_MACHINE_PREDICTOR_H
