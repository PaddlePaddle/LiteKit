package com.baidu.mmlcore;

public class MMLTensorShape {
    public int inputBatch;
    public int inputChannel;
    public int inputHeight;
    public int inputWidth;

    public MMLTensorShape(int batch, int channel, int height, int width) {
        inputBatch = batch;
        inputChannel = channel;
        inputHeight = height;
        inputWidth = width;
    }
}
