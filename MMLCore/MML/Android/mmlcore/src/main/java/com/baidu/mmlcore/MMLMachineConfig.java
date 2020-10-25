package com.baidu.mmlcore;

public class MMLMachineConfig {
    /**
     * 模型文件路径
     */
    public String modelPath;

    public enum MachineType {
        MMLPaddleLite(2);

        private MachineType(int value) {
            this.value = value;
        }
        public final int value;
    }

    /**
     * 预测引擎类型
     */
    public MachineType machineType;

    /**
     * machine_config
     */
    public MMLInferenceEngineConfig engineConifg;
}
