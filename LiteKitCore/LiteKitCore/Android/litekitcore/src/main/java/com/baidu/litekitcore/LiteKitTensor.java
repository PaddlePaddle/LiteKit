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

public class LiteKitTensor {
    /**
     * 保存native层的tensor的指针
     */
    private long nativeTensorHandler = 0L;

    /**
     * Deletes C++ Tensor pointer when Java LiteKitTensor object is destroyed
     */
    @Override
    protected void finalize() throws Throwable {
        if (nativeTensorHandler != 0L) {
            release(nativeTensorHandler);
            nativeTensorHandler = 0L;
        }
        super.finalize();
    }

    /**
     * Delete C++ Tenor object pointed by the input pointer, which is presented by a
     * long value.
     */
    private native void release(long nativeTensorHandler);

    /**
     * getter and setter
     */
    public void setNativeTensorHandler(long nativeTensorHandler) {
        this.nativeTensorHandler = nativeTensorHandler;
    }
    public long getNativeTensorHandler() {
        return nativeTensorHandler;
    }

    /**
     * fetch tensor shape or data
     */
    public long[] fetchShape() {
        return fetchShape(nativeTensorHandler);
    }
    public float[] fetchFloatData() {
        return fetchFloatData(nativeTensorHandler);
    }
    public int[] fetchIntData() {
        return fetchIntData(nativeTensorHandler);
    }
    private native long[] fetchShape(long nativeTensorHandler);
    private native float[] fetchFloatData(long nativeTensorHandler);
    private native int[] fetchIntData(long nativeTensorHandler);
}