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


#import "LiteKitMachineService.h"
#import "LiteKitBaseMachine+Logger.h"
#import "LiteKitPaddleConfig.h"
#import "LiteKitPaddleGPUMachine.h"
#import "LiteKitPaddleCPUMachine.h"

/// fluid模型固定模型文件名称
static NSString * const LiteKitPaddleModelName = @"model.mlm";
/// fluid模型固定参数文件名称
static NSString * const LiteKitPaddleParamName = @"params.mlm";

/// load阶段错误domain
NSString * const LiteKitMachineServiceLoadErrorDomain = @"LiteKitMachineServiceLoadErrorDomain";

/// error信息中的附带信息key值
NSString * const LiteKitMachineServiceErrorExtKey = @"machine_service_error_key";

/// load阶段错误Ext key
NSString * const LiteKitMachineServiceRunErrorExtKey = @"LiteKitMachineServiceRunErrorExtKey";


@interface LiteKitMachineService ()
/// 当前对应的machine
@property (nonatomic, strong) LiteKitBaseMachine * __nullable litekitMachine;

/// 当前的machineType
@property (nonatomic, assign) LiteKitMachineType currentMachineType;

/// 当前的machineConfig
@property (nonatomic, strong) LiteKitMachineConfig * __nullable currentMachineConfig;

/// Service的日志器
@property (nonatomic, strong) id <LiteKitLoggerProtocol> logger;

/// Service的唯一标识
@property (nonatomic, copy, readwrite) NSString *machineServiceId;

/// 同步锁
@property (nonatomic, strong) dispatch_semaphore_t lock;

/// 性能统计数据
@property (nonatomic, strong) LiteKitPerformanceProfiler *performanceDataMap;
@end

@implementation LiteKitMachineService

#pragma mark - Dealloc

- (void)dealloc {
    [self.logger warningLogMsg:@"Machine Service dealloc"];
}

#pragma mark - Public

/// sync
- (LiteKitBaseMachine *)loadMachineWithConfig:(LiteKitMachineConfig *)aConfig error:(NSError **)aError {
    [self setupLoggerFromMachineLoggerName:aConfig.loggerClassName];
    [self.logger debugLogMsg:@"load start"];
    [self litekit_createMachineWithConfig:aConfig error:aError];
    if (*aError) {
        [self.logger errorLogMsg:[NSString stringWithFormat:@"load error detail msg -- domain:%@, code:%ld, ext:%@", ( *aError).domain.description, ( *aError).code, ( *aError).userInfo]];
        _litekitMachine = nil;
    } else {
        [_litekitMachine setupMachineLoggerFromMachineLoggerName:aConfig.loggerClassName];
        [self.logger debugLogMsg:@"load success"];
    }
    
    return _litekitMachine;
}

/// async
- (void)loadMachineWithConfig:(LiteKitMachineConfig *)aConfig completionBlock:(LiteKitMachineLoadCompletionBlock)aBlock {
    [self setupLoggerFromMachineLoggerName:aConfig.loggerClassName];
    [self.logger debugLogMsg:@"load start"];
    NSError *error = nil;
    [self litekit_createMachineWithConfig:aConfig error:&error];
    if (error) {
        [self.logger errorLogMsg:[NSString stringWithFormat:@"load error detail msg -- domain:%@, code:%ld, ext:%@", error.domain.description, error.code, error.userInfo]];
        _litekitMachine = nil;
    } else {
        [_litekitMachine setupMachineLoggerFromMachineLoggerName:aConfig.loggerClassName];
        [self.logger debugLogMsg:@"load success"];
    }
    
    if (aBlock) {
        aBlock(self.litekitMachine, error);
    }
}


#pragma mark - Private

- (void)setupLoggerFromMachineLoggerName:(NSString *)loggerClsName {
    if ([loggerClsName isKindOfClass:[NSString class]] &&
        loggerClsName.length > 0 &&
        NSClassFromString(loggerClsName) &&
        [NSClassFromString(loggerClsName) instancesRespondToSelector:@selector(setLogTag:)]) {
        self.logger = [NSClassFromString(loggerClsName) new];
        [self.logger setLogTag:LiteKitMachineServiceLoggerTag];
    } else {
#if __has_include("LiteKitLogger.h")
        self.logger = [[LiteKitLogger alloc] initWithTag:LiteKitMachineServiceLoggerTag];
#endif
    }
}

#pragma mark - Create

- (void)litekit_createMachineWithConfig:(LiteKitMachineConfig *)aConfig error:(NSError **)aError {
    _currentMachineConfig = aConfig;
    _currentMachineType = aConfig.machineType;
    switch (self.currentMachineType) {
        case LiteKitPaddleGPU:
        case LiteKitPaddleCPU:
            [self litekit_createPaddleMachineWithConfig:aConfig error:aError];
            break;
        default:
            break;
    }
    
}

#pragma mark - Paddle

/// 创建统一Paddle Machine入口
/// @param aConfig LiteKit配置
/// @param aError 错误信息
- (void)litekit_createPaddleMachineWithConfig:(LiteKitMachineConfig *)aConfig
                                    error:(NSError **)aError {
    do {
#if TARGET_IPHONE_SIMULATOR
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadNotSupportSimulator userInfo:nil];
        }
        break;
#elif defined(__i386__)
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadNotSupportArchitecture userInfo:nil];
        }
        break;
#else
        if (self.currentMachineType == LiteKitPaddleGPU){
            [self litekit_createPaddleGPUMachineWithConfig:aConfig error:aError];
        } else if (self.currentMachineType == LiteKitPaddleCPU) {
            [self litekit_createPaddleCPUMachineWithConfig:aConfig error:aError];
        } else {
            if (aError != NULL) {
                *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadMachineTypeError userInfo:nil];
            }
        }
#endif
        
    } while (0);
}


/// 创建Paddle CPU
/// @param aConfig LiteKit配置
/// @param aError 错误信息
- (void)litekit_createPaddleCPUMachineWithConfig:(LiteKitMachineConfig *)aConfig
                                        error:(NSError **)aError {
#if __has_include("PaddleCPU.h")
    self.litekitMachine = [[LiteKitPaddleCPUMachine alloc] initWithModelDir:aConfig.modelPath
                                                           withError:aError];
    
#else
    NSLog(@"not import PaddleCPU.h");
    if (aError != NULL) {
        *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadMachineTypeError userInfo:@{LiteKitMachineServiceRunErrorExtKey:@"the project hadn't import PaddleCPU.h, so not support paddle CPU backend"}];
    }
#endif
}

/// 创建GPUMachine
/// @param aConfig LiteKit配置
/// @param aError 错误信息
- (void)litekit_createPaddleGPUMachineWithConfig:(LiteKitMachineConfig *)aConfig
                                       error:(NSError **)aError {
    if (!aConfig.engineConifg || ![aConfig.engineConifg conformsToProtocol:@protocol(LiteKitInferenceEngineConfigProtocol)]) {
           if (aError != NULL) {
               *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadWrongConfig userInfo:nil];
           }
           return;
    }
   LiteKitPaddleConfig *paddleEngineConfig = aConfig.engineConifg;

    unsigned char *modelPointer = paddleEngineConfig.modelPointer;
    unsigned char *paramPointer = paddleEngineConfig.paramPointer;
    if (aConfig.modelPath.length != 0) {
        NSError *error = nil;
        NSString *modelPath = [aConfig.modelPath stringByAppendingPathComponent:LiteKitPaddleModelName];
        NSString *paramPath = [aConfig.modelPath stringByAppendingPathComponent:LiteKitPaddleParamName];
        LiteKitPaddleGPUMachine *gpuMachine = [[LiteKitPaddleGPUMachine alloc] initWithModelPath:modelPath
                                                                               paramPath:paramPath
                                                                                 netType:paddleEngineConfig.netType
                                                                             modelConfig:paddleEngineConfig.paddleGPUConfig
                                                                               withError:&error];
        if (error != nil) {
            if (aError != NULL) {
                *aError = error;
            }
        } else {
            self.litekitMachine = gpuMachine;
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
        LiteKitPaddleGPUMachine *gpuMachine = [[LiteKitPaddleGPUMachine alloc] initWithModelConfig:paddleEngineConfig.paddleGPUConfig netType:paddleEngineConfig.netType withError:&error];
        if (error != nil) {
            if (aError != NULL) {
                *aError = error;
            }
        } else {
            self.litekitMachine = gpuMachine;
            [self.logger debugLogMsg:@"use pointer for load machine"];
        }
    } else {
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:LiteKitMachineServiceLoadErrorDomain code:LiteKitMachineServiceLoadNoModelPointer userInfo:nil];
        }
    }
}

#pragma mark - Getter

- (NSString *)machineServiceId {
    if (!_machineServiceId) {
        if (self.litekitMachine) {
            _machineServiceId = [NSString stringWithFormat:@"machine_service_%lu", (unsigned long)self.litekitMachine.hash];
        } else {
            _machineServiceId = nil;
        }
    }
    return _machineServiceId;
}

@end
