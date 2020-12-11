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


#import "LiteKitMachineService.h"
#import "LiteKitBaseMachine+Logger.h"
#import "LiteKitPaddleConfig.h"
#import "LiteKitPaddleGPUMachine.h"
#import "LiteKitPaddleCPUMachine.h"

/// fluid model default model file name
static NSString * const LiteKitPaddleModelName = @"model.mlm";
/// fluid model default param file name
static NSString * const LiteKitPaddleParamName = @"params.mlm";

/// load state errordomain
NSString * const LiteKitMachineServiceLoadErrorDomain = @"LiteKitMachineServiceLoadErrorDomain";

/// error message key
NSString * const LiteKitMachineServiceErrorExtKey = @"machine_service_error_key";

/// load state error key
NSString * const LiteKitMachineServiceRunErrorExtKey = @"LiteKitMachineServiceRunErrorExtKey";


@interface LiteKitMachineService ()
/// currrnt machine
@property (nonatomic, strong) LiteKitBaseMachine * __nullable litekitMachine;

/// current machineType
@property (nonatomic, assign) LiteKitMachineType currentMachineType;

/// current machineConfig
@property (nonatomic, strong) LiteKitMachineConfig * __nullable currentMachineConfig;

/// Service logger
@property (nonatomic, strong) id <LiteKitLoggerProtocol> logger;

/// Service identifier
@property (nonatomic, copy, readwrite) NSString *machineServiceId;

/// sync loock
@property (nonatomic, strong) dispatch_semaphore_t lock;

/// performance data
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

/// unify Paddle Machine entrance
/// @param aConfig LiteKit config
/// @param aError error message
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


/// create Paddle CPU
/// @param aConfig LiteKit config
/// @param aError error message
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

/// create GPUMachine
/// @param aConfig LiteKit config
/// @param aError error message
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
            paddleEngineConfig.paddleGPUConfig.paramPointer == NULL) { // business not set pointer in config
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
