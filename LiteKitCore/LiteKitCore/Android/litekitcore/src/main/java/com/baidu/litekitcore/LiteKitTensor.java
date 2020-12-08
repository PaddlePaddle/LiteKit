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