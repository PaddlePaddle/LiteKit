package com.baidu.mmlcore;

public class MMLPaddleLiteConfig {
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
