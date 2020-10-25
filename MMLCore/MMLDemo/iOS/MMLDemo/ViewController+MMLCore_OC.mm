//
//  ViewController+Load_Shared.m
//  MMLDemo_SourceCode
//
//  Created by Baidu Co.Ltd. on 2020/3/17.
//  Copyright © 2020 Baidu. All rights reserved.
//

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
