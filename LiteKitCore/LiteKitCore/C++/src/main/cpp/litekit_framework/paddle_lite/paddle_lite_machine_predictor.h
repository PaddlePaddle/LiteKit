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
    int load(const litekit_framework::LiteKitConfig &config);

    /**
     * 预测函数
     * @param inputData
     * @param outputData
     * @return
     */
    int predict(litekit_framework::LiteKitData &inputData, litekit_framework::LiteKitData *outputData);
    /**
    * 预测函数 适用于paddle-lite backend
    * @return i 0成功, 其他错误见litekit_framework ErrorCode
    */
    int predict();
    
    /**
    * 申请第i个input的tensor空间
    * @param i
    * @return LiteKitData
    */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputData(int i);
    
    /**
    * 获取多输出中的第i个output
    * @param i
    * @return LiteKitData
    */
    virtual std::unique_ptr<const litekit_framework::LiteKitData> getOutputData(int i);
    
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
     * @return LiteKitData
     */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputByName(const std::string& name) ;
    
    /**
     * 析构函数
     */
    ~PaddleLiteMachinePredictor();

};


#endif //LIB_AI_PADDLE_LITE_MACHIN_EPREDICTOR_H
