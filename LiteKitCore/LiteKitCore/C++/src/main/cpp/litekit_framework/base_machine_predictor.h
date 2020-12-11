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
 * BaseMachinePredictor ，different Backend（example:PaddleLite）should inherit and impl BaseMachinePredictor
 */

#ifndef LIB_AI_BASE_MACHINE_PREDICTOR_H
#define LIB_AI_BASE_MACHINE_PREDICTOR_H

#include "litekit_inference_api.h"

class BaseMachinePredictor {
public:
    // real pointer of BackendPredictor（example:PaddleLite）
    void *realPredictor;
    // backend的share pointer
    std::shared_ptr<void> realPredictorPtr;

    /**
     * according to config，create realPredictor，and load model
     *
     * @param config config
     * @return ErrorCode
     */
    virtual int load(const litekit_framework::LiteKitConfig &config) = 0;

    /**
     * predict function
     * @param inputData input data
     * @param outputData output data
     * @return ErrorCode
     */
    virtual int predict(litekit_framework::LiteKitData &inputData, litekit_framework::LiteKitData *outputData) = 0;
   
    /**
    * predict function for paddle-lite backend
    * @return ErrorCode, 0 means succeed, else see litekit_framework ErrorCode
    */
    virtual int predict() = 0;
    
    /**
       * create the Ith input tensor
       * @param i index
       * @return LiteKitData
       */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputData(int i) = 0;
    /**
       * get thr Ith output
       * @param i index
       * @return LiteKitData
       */
    virtual std::unique_ptr<const litekit_framework::LiteKitData> getOutputData(int i) = 0;
    /**
     * get inputnames
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getInputNames() = 0 ;
    /**
     * get output names
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getOutputNames() = 0 ;
    /**
     * get input by name
     * @param name input name
     * @return LiteKitData
     */
    virtual std::unique_ptr<litekit_framework::LiteKitData> getInputByName(const std::string& name) = 0 ;
    
    /**
     * Destructor。will delete realPredictor and set null。
     */
    virtual ~BaseMachinePredictor() {};
};


#endif //LIB_AI_BASE_MACHINE_PREDICTOR_H
