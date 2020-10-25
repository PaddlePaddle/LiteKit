package com.baidu.mmlcore;

public class MMLMachineService {

    public static MMLBaseMachine loadMachineWithConfig(MMLMachineConfig aConfig) {
        MMLBaseMachine machine = null;
        switch (aConfig.machineType) {
            case MMLPaddleLite: {
                machine = new MMLPaddleLiteMachine(aConfig);
                break;
            }
            default: {

            }
        }
        return machine;
    }
}
