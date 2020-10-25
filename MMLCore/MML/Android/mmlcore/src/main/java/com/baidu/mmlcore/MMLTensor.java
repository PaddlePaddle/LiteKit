package com.baidu.mmlcore;

public class MMLTensor {
    /**
     * 保存native层的tensor的指针
     */
    private long nativeTensorHandler = 0L;

    /**
     * Deletes C++ Tensor pointer when Java MMLTensor object is destroyed
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