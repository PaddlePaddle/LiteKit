// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import "ViewController+MMLCore_OC.h"

@implementation ViewController(MMLCore_OC)

- (MMLBaseMachine *)loadMMLWithModelDir_GPU:(NSString *)modelDir {
    PaddleGPUConfig *modelConfig = [[PaddleGPUConfig alloc] init];
    modelConfig.dims = @[@(kMMLInputBatch), @(kMMLInputChannel), @(kMMLInputHeight), @(kMMLInputWidth)];
    modelConfig.useMPS = YES;
    modelConfig.useAggressiveOptimization = YES;
    modelConfig.computePrecision = PrecisionTypeFloat16;
    modelConfig.modelPrecision = PrecisionTypeFloat32;
    modelConfig.metalLibPath = [NSBundle.mainBundle pathForResource:@"paddle-mobile-metallib" ofType:@"metallib"];
    modelConfig.metalLoadType = LoadMetalInCustomMetalLib;
    
    // construct inf engine config
    MMLPaddleConfig *paddleConfig = [[MMLPaddleConfig alloc] init];
    paddleConfig.netType = CustomNetType;
    paddleConfig.paddleGPUConfig = modelConfig;
    
    // construct mml config
    MMLMachineConfig *machineConfig = [[MMLMachineConfig alloc] init];
    machineConfig.modelPath = [modelDir stringByAppendingPathComponent:@"mobilenetv2"];
    machineConfig.engineConifg = paddleConfig;
    machineConfig.machineType = MMLPaddleGPU;

    // 加载模型
    NSError *aError = nil;
    MMLMachineService *service = [[MMLMachineService alloc] init];
    MMLBaseMachine *mmlMachine = [service loadMachineWithConfig:machineConfig error:&aError];
    
    return mmlMachine;
}

- (MMLBaseMachine *)loadMMLWithModelDir_CPU:(NSString *)modelDir {
    // construct mml config
    MMLMachineConfig *machineConfig = [[MMLMachineConfig alloc] init];
    machineConfig.modelPath = [modelDir stringByAppendingPathComponent:@"mobilenet_v1_opt.nb"];
    machineConfig.machineType = MMLPaddleCPU;

    // 加载模型
    NSError *aError = nil;
    MMLMachineService *service = [[MMLMachineService alloc] init];
    MMLBaseMachine *mmlMachine = [service loadMachineWithConfig:machineConfig error:&aError];
    
    return mmlMachine;
}

@end
