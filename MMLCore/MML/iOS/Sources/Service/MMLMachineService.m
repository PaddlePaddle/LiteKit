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


#import "MMLMachineService.h"
#import "MMLBaseMachine+Logger.h"
#import "MMLPaddleConfig.h"
#import "MMLPaddleGPUMachine.h"
#import "MMLPaddleCPUMachine.h"

/// fluid模型固定模型文件名称
static NSString * const MMLPaddleModelName = @"model.mlm";
/// fluid模型固定参数文件名称
static NSString * const MMLPaddleParamName = @"params.mlm";

/// load阶段错误domain
NSString * const MMLMachineServiceLoadErrorDomain = @"MMLMachineServiceLoadErrorDomain";

/// error信息中的附带信息key值
NSString * const MMLMachineServiceErrorExtKey = @"machine_service_error_key";

/// load阶段错误Ext key
NSString * const MMLMachineServiceRunErrorExtKey = @"MMLMachineServiceRunErrorExtKey";


@interface MMLMachineService ()
/// 当前对应的machine
@property (nonatomic, strong) MMLBaseMachine * __nullable mmlMachine;

/// 当前的machineType
@property (nonatomic, assign) MMLMachineType currentMachineType;

/// 当前的machineConfig
@property (nonatomic, strong) MMLMachineConfig * __nullable currentMachineConfig;

/// Service的日志器
@property (nonatomic, strong) id <MMLLoggerProtocol> logger;

/// Service的唯一标识
@property (nonatomic, copy, readwrite) NSString *machineServiceId;

/// 同步锁
@property (nonatomic, strong) dispatch_semaphore_t lock;

/// 性能统计数据
@property (nonatomic, strong) MMLPerformanceProfiler *performanceDataMap;
@end

@implementation MMLMachineService

#pragma mark - Dealloc

- (void)dealloc {
    [self.logger warningLogMsg:@"Machine Service dealloc"];
}

#pragma mark - Public

/// sync
- (MMLBaseMachine *)loadMachineWithConfig:(MMLMachineConfig *)aConfig error:(NSError **)aError {
    [self setupLoggerFromMachineLoggerName:aConfig.loggerClassName];
    [self.logger debugLogMsg:@"load start"];
    [self mml_createMachineWithConfig:aConfig error:aError];
    if (*aError) {
        [self.logger errorLogMsg:[NSString stringWithFormat:@"load error detail msg -- domain:%@, code:%ld, ext:%@", ( *aError).domain.description, ( *aError).code, ( *aError).userInfo]];
        _mmlMachine = nil;
    } else {
        [_mmlMachine setupMachineLoggerFromMachineLoggerName:aConfig.loggerClassName];
        [self.logger debugLogMsg:@"load success"];
    }
    
    return _mmlMachine;
}

/// async
- (void)loadMachineWithConfig:(MMLMachineConfig *)aConfig completionBlock:(MMLMachineLoadCompletionBlock)aBlock {
    [self setupLoggerFromMachineLoggerName:aConfig.loggerClassName];
    [self.logger debugLogMsg:@"load start"];
    NSError *error = nil;
    [self mml_createMachineWithConfig:aConfig error:&error];
    if (error) {
        [self.logger errorLogMsg:[NSString stringWithFormat:@"load error detail msg -- domain:%@, code:%ld, ext:%@", error.domain.description, error.code, error.userInfo]];
        _mmlMachine = nil;
    } else {
        [_mmlMachine setupMachineLoggerFromMachineLoggerName:aConfig.loggerClassName];
        [self.logger debugLogMsg:@"load success"];
    }
    
    if (aBlock) {
        aBlock(self.mmlMachine, error);
    }
}


#pragma mark - Private

- (void)setupLoggerFromMachineLoggerName:(NSString *)loggerClsName {
    if ([loggerClsName isKindOfClass:[NSString class]] &&
        loggerClsName.length > 0 &&
        NSClassFromString(loggerClsName) &&
        [NSClassFromString(loggerClsName) instancesRespondToSelector:@selector(setLogTag:)]) {
        self.logger = [NSClassFromString(loggerClsName) new];
        [self.logger setLogTag:MMLMachineServiceLoggerTag];
    } else {
#if __has_include("MMLLogger.h")
        self.logger = [[MMLLogger alloc] initWithTag:MMLMachineServiceLoggerTag];
#endif
    }
}

#pragma mark - Create

- (void)mml_createMachineWithConfig:(MMLMachineConfig *)aConfig error:(NSError **)aError {
    _currentMachineConfig = aConfig;
    _currentMachineType = aConfig.machineType;
    switch (self.currentMachineType) {
        case MMLPaddleGPU:
        case MMLPaddleCPU:
            [self mml_createPaddleMachineWithConfig:aConfig error:aError];
            break;
        default:
            break;
    }
    
}

#pragma mark - Paddle

/// 创建统一Paddle Machine入口
/// @param aConfig MML配置
/// @param aError 错误信息
- (void)mml_createPaddleMachineWithConfig:(MMLMachineConfig *)aConfig
                                    error:(NSError **)aError {
    do {
#if TARGET_IPHONE_SIMULATOR
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadNotSupportSimulator userInfo:nil];
        }
        break;
#elif defined(__i386__)
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadNotSupportArchitecture userInfo:nil];
        }
        break;
#else
        if (self.currentMachineType == MMLPaddleGPU){
            [self mml_createPaddleGPUMachineWithConfig:aConfig error:aError];
        } else if (self.currentMachineType == MMLPaddleCPU) {
            [self mml_createPaddleCPUMachineWithConfig:aConfig error:aError];
        } else {
            if (aError != NULL) {
                *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadMachineTypeError userInfo:nil];
            }
        }
#endif
        
    } while (0);
}


/// 创建Paddle CPU
/// @param aConfig MML配置
/// @param aError 错误信息
- (void)mml_createPaddleCPUMachineWithConfig:(MMLMachineConfig *)aConfig
                                        error:(NSError **)aError {
#if __has_include("PaddleCPU.h")
    self.mmlMachine = [[MMLPaddleCPUMachine alloc] initWithModelDir:aConfig.modelPath
                                                           withError:aError];
    
#else
    NSLog(@"not import PaddleCPU.h");
    if (aError != NULL) {
        *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadMachineTypeError userInfo:@{MMLMachineServiceRunErrorExtKey:@"the project hadn't import PaddleCPU.h, so not support paddle CPU backend"}];
    }
#endif
}

/// 创建GPUMachine
/// @param aConfig MML配置
/// @param aError 错误信息
- (void)mml_createPaddleGPUMachineWithConfig:(MMLMachineConfig *)aConfig
                                       error:(NSError **)aError {
    if (!aConfig.engineConifg || ![aConfig.engineConifg conformsToProtocol:@protocol(MMLInferenceEngineConfigProtocol)]) {
           if (aError != NULL) {
               *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadWrongConfig userInfo:nil];
           }
           return;
    }
   MMLPaddleConfig *paddleEngineConfig = aConfig.engineConifg;

    unsigned char *modelPointer = paddleEngineConfig.modelPointer;
    unsigned char *paramPointer = paddleEngineConfig.paramPointer;
    if (aConfig.modelPath.length != 0) {
        NSError *error = nil;
        NSString *modelPath = [aConfig.modelPath stringByAppendingPathComponent:MMLPaddleModelName];
        NSString *paramPath = [aConfig.modelPath stringByAppendingPathComponent:MMLPaddleParamName];
        MMLPaddleGPUMachine *gpuMachine = [[MMLPaddleGPUMachine alloc] initWithModelPath:modelPath
                                                                               paramPath:paramPath
                                                                                 netType:paddleEngineConfig.netType
                                                                             modelConfig:paddleEngineConfig.paddleGPUConfig
                                                                               withError:&error];
        if (error != nil) {
            if (aError != NULL) {
                *aError = error;
            }
        } else {
            self.mmlMachine = gpuMachine;
            [self.logger debugLogMsg:@"use pointer for load machine"];
        }
    }
    else if (modelPointer != NULL || paramPointer != NULL) {
        if (paddleEngineConfig.paddleGPUConfig.modelPointer == NULL ||
            paddleEngineConfig.paddleGPUConfig.paramPointer == NULL) { // 业务方未给config中的pointer赋值
            paddleEngineConfig.paddleGPUConfig.modelSize = (int)paddleEngineConfig.modelSize;
            paddleEngineConfig.paddleGPUConfig.paramSize = (int)paddleEngineConfig.paramSize;
            paddleEngineConfig.paddleGPUConfig.modelPointer = modelPointer;
            paddleEngineConfig.paddleGPUConfig.paramPointer = paramPointer;
        }
        
        NSError *error = nil;
        MMLPaddleGPUMachine *gpuMachine = [[MMLPaddleGPUMachine alloc] initWithModelConfig:paddleEngineConfig.paddleGPUConfig netType:paddleEngineConfig.netType withError:&error];
        if (error != nil) {
            if (aError != NULL) {
                *aError = error;
            }
        } else {
            self.mmlMachine = gpuMachine;
            [self.logger debugLogMsg:@"use pointer for load machine"];
        }
    } else {
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLMachineServiceLoadErrorDomain code:MMLMachineServiceLoadNoModelPointer userInfo:nil];
        }
    }
}

#pragma mark - Getter

- (NSString *)machineServiceId {
    if (!_machineServiceId) {
        if (self.mmlMachine) {
            _machineServiceId = [NSString stringWithFormat:@"machine_service_%lu", (unsigned long)self.mmlMachine.hash];
        } else {
            _machineServiceId = nil;
        }
    }
    return _machineServiceId;
}

@end
