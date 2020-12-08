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

#import "ViewController+LiteKitCore_OC.h"

@implementation ViewController(LiteKitCore_OC)

- (LiteKitBaseMachine *)loadLiteKitWithModelDir_GPU:(NSString *)modelDir {
    PaddleGPUConfig *modelConfig = [[PaddleGPUConfig alloc] init];
    modelConfig.dims = @[@(kLiteKitInputBatch), @(kLiteKitInputChannel), @(kLiteKitInputHeight), @(kLiteKitInputWidth)];
    modelConfig.useMPS = YES;
    modelConfig.useAggressiveOptimization = YES;
    modelConfig.computePrecision = PrecisionTypeFloat16;
    modelConfig.modelPrecision = PrecisionTypeFloat32;
    modelConfig.metalLibPath = [NSBundle.mainBundle pathForResource:@"paddle-mobile-metallib" ofType:@"metallib"];
    modelConfig.metalLoadType = LoadMetalInCustomMetalLib;
    
    // construct inf engine config
    LiteKitPaddleConfig *paddleConfig = [[LiteKitPaddleConfig alloc] init];
    paddleConfig.netType = CustomNetType;
    paddleConfig.paddleGPUConfig = modelConfig;
    
    // construct litekit config
    LiteKitMachineConfig *machineConfig = [[LiteKitMachineConfig alloc] init];
    machineConfig.modelPath = [modelDir stringByAppendingPathComponent:@"mobilenetv2"];
    machineConfig.engineConifg = paddleConfig;
    machineConfig.machineType = LiteKitPaddleGPU;

    // 加载模型
    NSError *aError = nil;
    LiteKitMachineService *service = [[LiteKitMachineService alloc] init];
    LiteKitBaseMachine *litekitMachine = [service loadMachineWithConfig:machineConfig error:&aError];
    
    return litekitMachine;
}

- (LiteKitBaseMachine *)loadLiteKitWithModelDir_CPU:(NSString *)modelDir {
    // construct litekit config
    LiteKitMachineConfig *machineConfig = [[LiteKitMachineConfig alloc] init];
    machineConfig.modelPath = [modelDir stringByAppendingPathComponent:@"mobilenet_v1_opt.nb"];
    machineConfig.machineType = LiteKitPaddleCPU;

    // 加载模型
    NSError *aError = nil;
    LiteKitMachineService *service = [[LiteKitMachineService alloc] init];
    LiteKitBaseMachine *litekitMachine = [service loadMachineWithConfig:machineConfig error:&aError];
    
    return litekitMachine;
}

@end
