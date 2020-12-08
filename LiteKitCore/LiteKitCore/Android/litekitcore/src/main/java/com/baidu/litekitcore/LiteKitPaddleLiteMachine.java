/*
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

package com.baidu.litekitcore;

import java.util.ArrayList;

public class LiteKitPaddleLiteMachine extends LiteKitBaseMachine {
    /**
     * Constructor of a LiteKitPaddleLiteMachine.
     *
     * @param config the input configuration.
     */
    public LiteKitPaddleLiteMachine(LiteKitMachineConfig config) {
        LiteKitPaddleConfig paddleConfig = (LiteKitPaddleConfig) config.engineConifg;
        LiteKitPaddleLiteConfig liteConfig = paddleConfig.liteConfig;
        // native
        nativeMachineHandler = createLiteKitMachineService(config.machineType.value, config.modelPath, liteConfig.model_type.value, liteConfig.threads, liteConfig.powermode.value);
    }

    @Override
    public ArrayList<LiteKitData> predictWithInputData(ArrayList<LiteKitData> aInputData) {
        // feed
        for (int i = 0; i < aInputData.size(); ++i) {
            LiteKitData data = aInputData.get(i);
            switch (data.getType()) {
                case kFLOAT: {
                    feedInputData(nativeMachineHandler, data.getFloatData(),
                            data.getShape().inputBatch, data.getShape().inputChannel, data.getShape().inputHeight, data.getShape().inputWidth,
                            data.getInputGraphId());
                    break;
                }
                case kBYTE: {
                    feedInputData(nativeMachineHandler, data.getByteData(),
                            data.getShape().inputBatch, data.getShape().inputChannel, data.getShape().inputHeight, data.getShape().inputWidth,
                            data.getInputGraphId());
                    break;
                }
                case kINT: {
                    feedInputData(nativeMachineHandler, data.getIntData(),
                            data.getShape().inputBatch, data.getShape().inputChannel, data.getShape().inputHeight, data.getShape().inputWidth,
                            data.getInputGraphId());
                    break;
                }
                case kLONG: {
                    feedInputData(nativeMachineHandler, data.getLongData(),
                            data.getShape().inputBatch, data.getShape().inputChannel, data.getShape().inputHeight, data.getShape().inputWidth,
                            data.getInputGraphId());
                    break;
                }
            }
        }
        // run
        run(nativeMachineHandler);
        // fetch
        ArrayList<LiteKitData> ret = new ArrayList<>();
        int count = fetchOutputCount(nativeMachineHandler);
        for (int i = 0; i < count; ++i) {
            LiteKitData data = new LiteKitData();
            data.output.setNativeTensorHandler(fetchOutputLiteKitTensor(nativeMachineHandler, i));
            ret.add(data);
        }
        return ret;
    }

    /**
     * create LiteKit machine service
     */
    public native long createLiteKitMachineService(int backEndType, String modelPath, int modelType, int thread, int powerMode);

    /**
     * feed input data
     */
    public native void feedInputData(long nativeMachineHandler, float[] inputData, int inputBatch, int inputChannel, int inputHeight, int inputWidth, int inputGraphId);
    public native void feedInputData(long nativeMachineHandler, byte[] inputData, int inputBatch, int inputChannel, int inputHeight, int inputWidth, int inputGraphId);
    public native void feedInputData(long nativeMachineHandler, int[] inputData, int inputBatch, int inputChannel, int inputHeight, int inputWidth, int inputGraphId);
    public native void feedInputData(long nativeMachineHandler, long[] inputData, int inputBatch, int inputChannel, int inputHeight, int inputWidth, int inputGraphId);

    /**
     * fetch output
     */
    public native int fetchOutputCount(long nativeMachineHandler);
    public native long fetchOutputLiteKitTensor(long nativeMachineHandler, int outputGraphId);
}