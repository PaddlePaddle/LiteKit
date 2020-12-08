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

public class LiteKitPaddleLiteConfig {
    public enum PaddleLitePowerMode {
        LITE_POWER_HIGH(0),
        LITE_POWER_LOW(1),
        LITE_POWER_FULL(2),
        LITE_POWER_NO_BIND(3),
        LITE_POWER_RAND_HIGH(4),
        LITE_POWER_RAND_LOW(5);

        private PaddleLitePowerMode(int value) {
            this.value = value;
        }
        public final int value;
    }

    public PaddleLitePowerMode powermode = PaddleLitePowerMode.LITE_POWER_NO_BIND;
    public int threads = 1;

    enum PaddleLiteModelType {
        LITE_MODEL_FROM_DIR(0),
        LITE_MODEL_FROM_FILE(1),
        LITE_MODEL_FROM_BUFFER(2),
        LITE_MODEL_FROM_MODELBUFFER(3);

        private PaddleLiteModelType(int value) {
            this.value = value;
        }
        public final int value;
    }

    public PaddleLiteModelType model_type = PaddleLiteModelType.LITE_MODEL_FROM_FILE;
}
