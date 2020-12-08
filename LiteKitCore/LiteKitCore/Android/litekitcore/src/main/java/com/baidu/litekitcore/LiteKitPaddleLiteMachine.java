/* Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

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