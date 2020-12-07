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
 * PaddleGPUMachinePredictor封装，实现基于PaddleLite的预测引擎
 */
#ifndef LIB_AI_PADDLE_GPU_MACHIN_EPREDICTOR_H
#define LIB_AI_PADDLE_GPU_MACHIN_EPREDICTOR_H


#include "../base_machine_predictor.h"

class PaddleGPUMachinePredictor : public BaseMachinePredictor {

public:
    /**
     * 根据config，创建realPredictor，并加载模型
     *
     * @param config 共有及自有配置
     * @return 加载成功或失败
     */
    int load(const mml_framework::MMLConfig &config);

    /**
     * 预测函数
     * @param inputData 预测输入
     * @param outputData 预测输出
     * @return 预测成功或失败
     */
    int predict(mml_framework::MMLData &inputData, mml_framework::MMLData *outputData);
    
    /**
    * 预测函数 适用于paddle-lite backend
    * @return i 0成功, 其他错误见mml_framework ErrorCode
    */
    int predict();
    /**
    * 申请第i个input的tensor空间
    * @param i 第i个输入
    * @return MMLData
    */
    virtual std::unique_ptr<mml_framework::MMLData> getInputData(int i) ;
    /**
    * 获取多输出中的第i个output
    * @param i 第i个输出
    * @return MMLData
    */
    virtual std::unique_ptr<const mml_framework::MMLData> getOutputData(int i) ;
    /**
    * 获取input的names
    * @return std::vector<std::string>
    */
    virtual std::vector<std::string> getInputNames() ;
    /**
    * 获取output的names
    * @return std::vector<std::string>
    */
    virtual std::vector<std::string> getOutputNames() ;
    /**
    * 获取name对应的input
    * @param name input的name
    * @return std::unique_ptr<mml_framework::MMLTensor>
    */
    virtual std::unique_ptr<mml_framework::MMLData> getInputByName(const std::string& name) ;
    /**
     * 析构函数。会delete realPredictor 并置空。
     */
    ~PaddleGPUMachinePredictor();

};


#endif //LIB_AI_PADDLE_GPU_MACHIN_EPREDICTOR_H
