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
 * PaddleLiteMachinePredictor封装，实现基于PaddleLite的预测引擎
 */
#ifndef LIB_AI_PADDLE_LITE_MACHIN_EPREDICTOR_H
#define LIB_AI_PADDLE_LITE_MACHIN_EPREDICTOR_H


#include "../base_machine_predictor.h"

class PaddleLiteMachinePredictor : public BaseMachinePredictor {

public:
    /**
     * 根据config，创建对应backend，并且完成对 backend的实例化
     */
    int load(const mml_framework::MMLConfig &config);

    /**
     * 预测函数
     * @param inputData
     * @param outputData
     * @return
     */
    int predict(mml_framework::MMLData &inputData, mml_framework::MMLData *outputData);
    /**
    * 预测函数 适用于paddle-lite backend
    * @return i 0成功, 其他错误见mml_framework ErrorCode
    */
    int predict();
    
    /**
    * 申请第i个input的tensor空间
    * @param i
    * @return MMLData
    */
    virtual std::unique_ptr<mml_framework::MMLData> getInputData(int i);
    
    /**
    * 获取多输出中的第i个output
    * @param i
    * @return MMLData
    */
    virtual std::unique_ptr<const mml_framework::MMLData> getOutputData(int i);
    
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
     * @return MMLData
     */
    virtual std::unique_ptr<mml_framework::MMLData> getInputByName(const std::string& name) ;
    
    /**
     * 析构函数
     */
    ~PaddleLiteMachinePredictor();

};


#endif //LIB_AI_PADDLE_LITE_MACHIN_EPREDICTOR_H
