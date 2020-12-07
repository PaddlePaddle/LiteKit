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
 * BaseMachinePredictor封装，不同Backend实现（如PaddleLite）应当继承自BaseMachinePredictor
 */

#ifndef LIB_AI_BASE_MACHINE_PREDICTOR_H
#define LIB_AI_BASE_MACHINE_PREDICTOR_H

#include "litekit_inference_api.h"

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
    virtual int load(const litekit_framework::LiteKitConfig &config) = 0;

    /**
     * 预测函数
     * @param inputData
     * @param outputData
     * @return
     */
    virtual int predict(litekit_framework::LiteKitData &inputData, litekit_framework::LiteKitData *outputData) = 0;
    /**
    * 预测函数 适用于paddle-lite backend
    * @return i 0成功, 其他错误见litekit_framework ErrorCode
    */
    virtual int predict() = 0;
    /**
       * 申请第i个input的tensor空间
       * @param i
       * @return LiteKitData
       */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputData(int i) = 0;
    /**
       * 获取多输出中的第i个output
       * @param i
       * @return LiteKitData
       */
    virtual std::unique_ptr<const litekit_framework::LiteKitData> getOutputData(int i) = 0;
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
     * @return LiteKitData
     */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputByName(const std::string& name) = 0 ;
    
    /**
     * 析构函数。会delete realPredictor 并置空。
     */
    virtual ~BaseMachinePredictor() {};
};


#endif //LIB_AI_BASE_MACHINE_PREDICTOR_H
