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
