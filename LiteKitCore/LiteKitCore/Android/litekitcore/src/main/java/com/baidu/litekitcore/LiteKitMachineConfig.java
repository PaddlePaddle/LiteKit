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

public class LiteKitMachineConfig {
    /**
     * 模型文件路径
     */
    public String modelPath;

    public enum MachineType {
        LiteKitPaddleLite(2);

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
    public LiteKitInferenceEngineConfig engineConifg;
}
