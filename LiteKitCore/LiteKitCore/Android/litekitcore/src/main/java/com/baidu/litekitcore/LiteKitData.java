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

import android.util.Log;

/**
 * LiteKitData include input and output data
 */
public class LiteKitData {
    public enum Type {
        kFLOAT,
        kBYTE,
        kINT,
        kLONG
    };

    /**
     * getter and setter
     */
    public Type getType() {
        return type;
    }

    public LiteKitTensorShape getShape() {
        return shape;
    }

    public int getInputGraphId() {
        return inputGraphId;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public void setShape(LiteKitTensorShape shape) {
        this.shape = shape;
    }

    public void setInputGraphId(int inputGraphId) {
        this.inputGraphId = inputGraphId;
    }

    /**
     * getter and setter
     */
    public float[] getFloatData() {
        return floatData;
    }

    public byte[] getByteData() {
        return byteData;
    }

    public int[] getIntData() {
        return intData;
    }

    public long[] getLongData() {
        return longData;
    }

    public void setFloatData(float[] floatData) {
        this.floatData = floatData;
    }

    public void setByteData(byte[] byteData) {
        this.byteData = byteData;
    }

    public void setIntData(int[] intData) {
        this.intData = intData;
    }

    public void setLongData(long[] longData) {
        this.longData = longData;
    }

    /**
     * input datas
     */
    private float[] floatData;
    private byte[] byteData;
    private int[] intData;
    private long[] longData;
    private Type type;
    private LiteKitTensorShape shape;
    private int inputGraphId;

    /**
     * output data
     */
    public LiteKitTensor output;

    /**
     * Constructor
     */
    public LiteKitData() {
        output = new LiteKitTensor();
    }
    public LiteKitData(float[] in, int batch, int channel, int height, int width, int id) {
        this.floatData = in;
        this.type = Type.kFLOAT;
        this.shape = new LiteKitTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new LiteKitTensor();
    }
    public LiteKitData(byte[] in, int batch, int channel, int height, int width, int id) {
        this.byteData = in;
        this.type = Type.kBYTE;
        this.shape = new LiteKitTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new LiteKitTensor();
    }
    public LiteKitData(int[] in, int batch, int channel, int height, int width, int id) {
        this.intData = in;
        this.type = Type.kINT;
        this.shape = new LiteKitTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new LiteKitTensor();
    }
    public LiteKitData(long[] in, int batch, int channel, int height, int width, int id) {
        this.longData = in;
        this.type = Type.kLONG;
        this.shape = new LiteKitTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new LiteKitTensor();
    }
}