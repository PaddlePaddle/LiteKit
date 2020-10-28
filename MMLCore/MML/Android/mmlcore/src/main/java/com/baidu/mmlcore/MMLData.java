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

package com.baidu.mmlcore;

import android.util.Log;

/**
 * MMLData包含输入和输出的数据
 */
public class MMLData {
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

    public MMLTensorShape getShape() {
        return shape;
    }

    public int getInputGraphId() {
        return inputGraphId;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public void setShape(MMLTensorShape shape) {
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
     * 输入数据相关
     */
    private float[] floatData;
    private byte[] byteData;
    private int[] intData;
    private long[] longData;
    private Type type;
    private MMLTensorShape shape;
    private int inputGraphId;

    /**
     * 输出数据相关
     */
    public MMLTensor output;

    /**
     * 构造函数
     */
    public MMLData() {
        output = new MMLTensor();
    }
    public MMLData(float[] in, int batch, int channel, int height, int width, int id) {
        this.floatData = in;
        this.type = Type.kFLOAT;
        this.shape = new MMLTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new MMLTensor();
    }
    public MMLData(byte[] in, int batch, int channel, int height, int width, int id) {
        this.byteData = in;
        this.type = Type.kBYTE;
        this.shape = new MMLTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new MMLTensor();
    }
    public MMLData(int[] in, int batch, int channel, int height, int width, int id) {
        this.intData = in;
        this.type = Type.kINT;
        this.shape = new MMLTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new MMLTensor();
    }
    public MMLData(long[] in, int batch, int channel, int height, int width, int id) {
        this.longData = in;
        this.type = Type.kLONG;
        this.shape = new MMLTensorShape(batch, channel, height, width);
        this.inputGraphId = id;

        this.output = new MMLTensor();
    }
}