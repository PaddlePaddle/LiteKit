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

abstract public class LiteKitBaseMachine {
    /**
     * Predict with input data
     *
     * */
    public abstract ArrayList<LiteKitData> predictWithInputData(ArrayList<LiteKitData> aInputData);

    /**
     * Run the Machine.
     *
     * */
    public native void run(long nativeMachineHandler);

    /**
     * Delete C++ Machine object pointed by the input pointer, which is
     * presented by a long value.
     *
     */
    public native void release(long nativeMachineHandler);

    /** name of C++ JNI lib */
    private final static String JNI_LIB_NAME = "litekit-core";

    /**
     * Initializes at the beginning
     * loads the C++ JNI lib. */
    static {
        System.loadLibrary(JNI_LIB_NAME);
    }

    /**
     * Java doesn't have pointer. To maintain the life cycle of underneath C++
     * Machine object, we use a long value to maintain it.
     */
    protected long nativeMachineHandler = 0L;

    /**
     * Deletes C++ Machine pointer when Java Machine object is
     * destroyed
     */
    @Override
    protected void finalize() throws Throwable {
        releaseMachine();
        super.finalize();
    }

    /**
     * Deletes C++ Machine pointer
     *
     */
    public void releaseMachine() {
        if (nativeMachineHandler != 0L) {
            release(nativeMachineHandler);
            nativeMachineHandler = 0L;
        }
    }
}
