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


#import "MMLPaddleGPUMachine.h"
#import <Metal/MTLTexture.h>
#import <Metal/Metal.h>
#import "MMLBaseMachine+Logger.h"
#import "MMLBaseMachine+Performancer.h"

#import "PaddleGPU+MetalResource.h"
#import "MMLInputConvertor.h"
#import "MMLOutputConvertor.h"

#include <string>
#include <iostream>

/// Paddle GPU 的Machine
@interface MMLPaddleGPUMachine ()
@property (nonatomic, strong) id<MTLDevice> device;
@property (nonatomic, strong) PaddleGPU *paddleGPU;
@property (nonatomic, strong) dispatch_semaphore_t semaphore;
@property (nonatomic, strong) PaddleGPUConfig *gpuConfig;
@end

@implementation MMLPaddleGPUMachine

/*
 * @brief Action = 初始化模型 模型操作工具Machine
 * @param - modelURL 模型文件URL（模型文件本身）
 * @param - paramPath params文件URL
 * @param - netType 网络类型
 * @param - config 原始ModelConfig
 * @param - error 错误容器 （ErrorDomain:MMLPaddleMachineInitErrorDomain ErrorCode:MMLMachineErrorCode）
 * @return instancetype
 */
- (instancetype __nullable)initWithModelPath:(NSString *)modelPath
                                   paramPath:(NSString *)paramPath
                                     netType:(NetType)netType
                                 modelConfig:(PaddleGPUConfig *)config
                                   withError:(NSError**)error {
    if (self = [super init]) {
        if ([modelPath isKindOfClass:[NSString class]]
            && ![modelPath isEqualToString:@""]
            && [paramPath isKindOfClass:[NSString class]]
            && ![paramPath isEqualToString:@""]) {
            NSFileManager *fileManager = [NSFileManager defaultManager];
            if ([fileManager fileExistsAtPath:modelPath] && [fileManager fileExistsAtPath:paramPath]){
                _semaphore = dispatch_semaphore_create(1);
                _gpuConfig = [self loadGPUModelConfigWithModelPath:modelPath paramPath:paramPath originConfig:config];
                
                if (!_gpuConfig) {
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLMachineInitFailed userInfo:nil];
                    }
                    return nil;
                }
                
                if (!SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) { // < 9.0
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLIllegalOSVersion userInfo:nil];
                    }
                    return nil;
                }
                
                _gpuConfig = [self mml_appendConfigWithMetalLibPathIfNeed:_gpuConfig netType:netType error:error];
                if (*error) {
                    return nil;
                }
                
                _paddleGPU = [[PaddleGPU alloc] initWithCommandQueue:[self paddleMTLCommandQueue]
                                                                       net:netType
                                                               modelConfig:_gpuConfig];
                if (!_paddleGPU) {
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLMachineInitFailed userInfo:nil];
                    }
                    return nil;
                }
                
                NSError *aError = nil;
                [self mml_loadMachine:&aError];
                
                if (_gpuConfig.modelPointer != NULL && _gpuConfig.paramPointer != NULL) {
                    free(_gpuConfig.modelPointer);
                    free(_gpuConfig.paramPointer);
                }
                
                if (aError) {
                    // error
                    self = nil;
                    if (error != NULL) {
                        *error = aError;
                    }
                }
            } else {
                self = nil;
                if (error != NULL) {
                    *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLModelFileNotFound userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, model file is not found -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (long)(*error).code, (*error).userInfo]];
                }
            }
        } else {
            //error
            self = nil;
            if (error != NULL) {
                *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLIllegalModelFileURL userInfo:nil];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, model file is error -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (*error).code, (*error).userInfo]];
            }
        }
    }
    
    return self;
}

/**
 初始化模型

 @param modelConfig modelconfig配置
 @param netType 网络类型
 @param error 错误容器
 @return instancetype
 */
- (instancetype __nullable)initWithModelConfig:(PaddleGPUConfig *)modelConfig
                                       netType:(NetType)netType
                                     withError:(NSError**)error {
    if (self = [super init]) {
        if (modelConfig.paramPointer != NULL
            && modelConfig.modelPointer != NULL
            && modelConfig.modelSize != 0
            && modelConfig.paramSize != 0
            && modelConfig != nil
            && [modelConfig isKindOfClass:[PaddleGPUConfig class]]) {
            _semaphore = dispatch_semaphore_create(1);
            _gpuConfig = modelConfig;
            if (!_gpuConfig) {
                if (error != NULL) {
                    *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLMachineInitFailed userInfo:nil];
                }
                return nil;
            }
            
            if (!SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) { // < 9.0
                if (error != NULL) {
                    *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLIllegalOSVersion userInfo:nil];
                }
                return nil;
            }
            
            _gpuConfig = [self mml_appendConfigWithMetalLibPathIfNeed:_gpuConfig netType:netType error:error];
            if (*error) {
                return nil;
            }
            
            _paddleGPU = [[PaddleGPU alloc] initWithCommandQueue:[self paddleMTLCommandQueue]
                                                                   net:netType
                                                           modelConfig:_gpuConfig];
            if (!_paddleGPU) {
                if (error != NULL) {
                    *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLMachineInitFailed userInfo:nil];
                }
                return nil;
            }
            
            NSError *aError = nil;
            [self mml_loadMachine:&aError];
            
            if (aError) {
                // error
                self = nil;
                if (error != NULL) {
                    *error = aError;
                }
            }
        } else {
            self = nil;
            if (error != NULL) {
                *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachineInitErrorCode)MMLIllegalModelConfig userInfo:nil];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, modelconfig is not correct -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (*error).code, (*error).userInfo]];
            }
        }
    }
    return self;
}

#pragma mark - Super

- (void)predictWithInputData:(MMLData * __nonnull)aInputData completionBlock:(MMLMachinePredictCompletionBlock)aBlock {
    if (!self) {
        NSError *error = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:MMLMachinePredicateMachineNotReady userInfo:nil];
        aBlock(nil, error);
        return;
    }
  
    switch (aInputData.type) {
        case TMMLDataTypeMTLTexture:{
            [self mml_internalPaddleGPUPredictWithTexture:aInputData.mtlTexture dims:aInputData.dims comletionBlock:aBlock];
            break;
        }
            
        case TMMLDataTypeMTLBuffer:{
            [self mml_internalPaddleGPUPredictWithBuffer:aInputData.mtlBuffer dims:aInputData.dims comletionBlock:aBlock];
            break;
        }
            
        default:{
            MMLInputMatrix *inputMatrix = [MMLInputConvertor convertMMLInputToPaddleInput:aInputData];
            [self mml_internalPaddleGPUPredictWithModelInputData:inputMatrix comletionBlock:aBlock];
            break;
        }
    }
    
}

- (void)releaseMachine {
    [self.paddleGPU clear];
    self.paddleGPU = nil;
}
/**
 清除machine,彻底释放内存
 */
- (void)clearMachine {
    [self.paddleGPU clear];
}


#pragma mark - Public

/**
 预测接口
 
 @param input 输入容器
 @param callback 结果容器
 */
- (void)predictWithBuffer:(id<MTLBuffer>)input
                     dims:(NSArray *)dims
          completionBlock:(MMLPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || [dims count] < 4
        || ![input conformsToProtocol:@protocol(MTLBuffer)] ) {
        NSError *aError = [NSError errorWithDomain:MMLMachinPredicateErrorDomain code:MMLMachinePredicateInputDataError userInfo:nil];
        callback(nil, aError);
        return;
    }
    
    if ( nil != dims ) {
        dispatch_semaphore_wait(self.semaphore, DISPATCH_TIME_FOREVER);
        NSError *aError = nil;
        @try {
            [self.paddleGPU updateInputDimInDim:dims error:&aError];
            if (aError) {
                callback(nil, aError);
                dispatch_semaphore_signal(self.semaphore);
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                return;
            }
        } @catch (NSException *exception) {
            aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateUpdateDimError userInfo:exception.userInfo];
            callback(nil, aError);
            dispatch_semaphore_signal(self.semaphore);
            [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, catch error update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
            return;
        }
        dispatch_semaphore_signal(self.semaphore);
    }
    
    if (self.paddleGPU) {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        [self.paddleGPU predictWithBuffer:input
                               channelNum:[dims.lastObject integerValue]
                withErrorResultCompletion:^(NSError * error, NSArray<PaddleGPUResult *> *result) {
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
            [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu predict time: %f ms", (end - start) * 1000]];
            self.predictTime = [NSString stringWithFormat:@"%.3f", (end - start) * 1000];
            if (error) {
                callback(nil, error);
            } else {
                NSError *aError = nil;
                if (!result || result.count <= 0) {
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                 
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // task执行完毕后，有线程的异步切换，导致此方法先执行，业务方获得的内存在使用时已经被释放，会有crash发生，所以在MMLShapedData内部对data进行copy，生命周期随着shapedData一起，这里就可以直接释放掉
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
             }
        }];
    } else {
        // error 模型已经释放
        NSError *aError = nil;
        aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}


/**
 预测接口
 
 @param input 输入容器
 @param callback 结果容器
 */
- (void)predictWithTexture:(id<MTLTexture>)input
                      dims:(NSArray *)dims
           completionBlock:(MMLPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || ![input conformsToProtocol:@protocol(MTLTexture)] ) {
        NSError *aError = [NSError errorWithDomain:MMLMachinPredicateErrorDomain code:MMLMachinePredicateInputDataError userInfo:nil];
        callback(nil, aError);
        return;
    }
    
    if ( nil != dims ) {
        dispatch_semaphore_wait(self.semaphore, DISPATCH_TIME_FOREVER);
        NSError *aError = nil;
        @try {
            [self.paddleGPU updateInputDimInDim:dims error:&aError];
            if (aError) {
                callback(nil, aError);
                dispatch_semaphore_signal(self.semaphore);
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                return;
            }
        } @catch (NSException *exception) {
            aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateUpdateDimError userInfo:exception.userInfo];
            callback(nil, aError);
            dispatch_semaphore_signal(self.semaphore);
            [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, catch error update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
            return;
        }
        dispatch_semaphore_signal(self.semaphore);
    }
    
    if (self.paddleGPU) {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        [self.paddleGPU predict:input withErrorResultCompletion:^(NSError *error, NSArray<PaddleGPUResult *> *result) {
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
            [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu predict time: %f ms", (end - start) * 1000]];
            self.predictTime = [NSString stringWithFormat:@"%.3f", (end - start) * 1000];
            if (error) {
                callback(nil, error);
            } else {
                NSError *aError = nil;
                if (!result || result.count <= 0) {
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // task执行完毕后，有线程的异步切换，导致此方法先执行，业务方获得的内存在使用时已经被释放，会有crash发生，所以在MMLShapedData内部对data进行copy，生命周期随着shapedData一起，这里就可以直接释放掉
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
            }
        }];
    } else {
        // error 模型已经释放
        NSError *aError = nil;
        aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}

/**
 预测接口
 
 @param input 输入容器
 @param callback 结果容器
 */
- (void)predictWithInputMatrix:(MMLInputMatrix *)input
               completionBlock:(MMLPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || ![input isKindOfClass:[MMLInputMatrix class]]
        || input.pixels == NULL
        || input.height <= 0
        || input.width <= 0) {
        NSError *aError = [NSError errorWithDomain:MMLMachinPredicateErrorDomain code:MMLMachinePredicateInputDataError userInfo:nil];
        callback(nil, aError);
        return;
    }
    
    if (self.paddleGPU) {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        // dims是 n h w c，内存排布输入输出都是nchw，所以input.pixels的排布为nchw
        // TODO:后续gpu不使用getTexture接口，直接使用predictWithBuffer的接口(PaddleGPU.m得补充此接口)
        NSArray *dims = [NSArray arrayWithObjects:@(1), [NSNumber numberWithInt:input.height], [NSNumber numberWithInt:input.width], [NSNumber numberWithInt:input.channel], nil];

        int w  = input.width;
        int h  = input.height;
        int channel = input.channel;
        
        id <MTLBuffer> buffer = [self.device newBufferWithLength:(w * h * channel * sizeof(float)) options:MTLResourceCPUCacheModeDefaultCache];
        memcpy(buffer.contents, input.pixels, (w * h * channel * sizeof(float)));
        dispatch_semaphore_wait(self.semaphore, DISPATCH_TIME_FOREVER);
        NSError *aError = nil;
        @try {
            [self.paddleGPU updateInputDimInDim:dims error:&aError];
            if (aError) {
                callback(nil, aError);
                // release buffer
                buffer = nil;
                dispatch_semaphore_signal(self.semaphore);
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                return;
            }
        } @catch (NSException *exception) {
            aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateUpdateDimError userInfo:exception.userInfo];
            callback(nil, aError);
            // release buffer
            buffer = nil;
            dispatch_semaphore_signal(self.semaphore);
            [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, catch error update dims error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
            return;
        }
        dispatch_semaphore_signal(self.semaphore);
        
        [self.paddleGPU predictWithBuffer:buffer
                               channelNum:channel
                withErrorResultCompletion:^(NSError * _Nullable error, NSArray<PaddleGPUResult *> * result) {
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
            [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu predict time: %f ms", (end - start) * 1000]];
            self.predictTime = [NSString stringWithFormat:@"%.3f", (end - start) * 1000];
            if (error) {
                callback(nil, error);
            } else {
                NSError *aError = nil;
                if (!result || result.count <= 0) {
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // task执行完毕后，有线程的异步切换，导致此方法先执行，业务方获得的内存在使用时已经被释放，会有crash发生，所以在MMLShapedData内部对data进行copy，生命周期随着shapedData一起，这里就可以直接释放掉
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
            }
        }];
        // release buffer
        buffer = nil;
    } else {
        // error 模型已经释放
        NSError *aError = nil;
        aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}

/**
 释放GPU且清除machine,释放内存
 */
- (void)releasePaddleGPU {
    [self.paddleGPU clear];
    self.paddleGPU = nil;
}

/**
 重新load machine
 */
- (void)reloadMachineWithError:(NSError **)error {
    if (self.paddleGPU) {
        [self mml_loadMachine:error];
    } else {
        *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachineLoadFailed userInfo:nil];
    }
}


#pragma mark - Private

/**
 load paddle gpu

 @param aError 加载paddle gpu的结果
 */
- (void)mml_loadMachine:(NSError **)aError {
    @try {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        [_paddleGPU loadWithError:aError];
        NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
        [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu load time: %f ms", (end - start) * 1000]];
        self.loadTime = [NSString stringWithFormat:@"%.3f", (end - start) * 1000];
    } @catch (NSException *exception) {
        *aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:(MMLMachinePredicateErrorCode)MMLMachinePredicateLoadError userInfo:@{NSLocalizedDescriptionKey:exception.description}];
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, machine load error -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
    }
}

/**
 添加metallib的config配置
 @param config 初始config
 @param netType net类型
 @param error error
 @return ModelConfig的实例
 */
- (PaddleGPUConfig *)mml_appendConfigWithMetalLibPathIfNeed:(PaddleGPUConfig *)config
                                                netType:(NetType)netType
                                                  error:(NSError **)error {
    do {
        if (!config || ![config isKindOfClass:[PaddleGPUConfig class]]) {
            break;
        }
        // 已经设置了自定义的metal路径，则不再加载默认的metal路径
        if (config.metalLibPath && config.metalLoadType == LoadMetalInCustomMetalLib) {
            break;
        }
        NSError *aError = nil;
        NSTimeInterval startTime = [[NSDate date] timeIntervalSince1970];
        NSString *paddleMetalLibPath = [PaddleGPU pm_customMetalLibResourceWithError:&aError];
        NSTimeInterval endTime = [[NSDate date] timeIntervalSince1970];
        self.readMetalResourceTime = [NSString stringWithFormat:@"%.3f", (endTime - startTime) * 1000];
        if (aError) {
            if (error != NULL) {
                NSNumber *codeNumber = [NSNumber numberWithInteger:aError.code];
                *error = [NSError errorWithDomain:MMLPaddleMachineInitErrorDomain code:(MMLMachinePredicateErrorCode)MMLIllegalModelConfig userInfo:@{aError.domain:codeNumber}];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, unzip metallib path error -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (*error).code, (*error).userInfo]];
            }
        } else {
            NSFileManager *fileManager = [NSFileManager defaultManager];
            if ([fileManager fileExistsAtPath:paddleMetalLibPath]) {
                config.metalLibPath = paddleMetalLibPath;
                config.metalLoadType = LoadMetalInCustomMetalLib;
            }
        }
    } while (0);
    
    return config;
}

/**
 加载一个新的GPU Config
 
 @return config实例
 */
- (PaddleGPUConfig *__nullable)loadGPUModelConfigWithModelPath:(NSString *)modelPath
                                                 paramPath:(NSString *)modelParamPath
                                              originConfig:(PaddleGPUConfig *)config {
    if (!config) {
        config = [[PaddleGPUConfig alloc] init];
    }
    
    do {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        if (![fileManager fileExistsAtPath:modelPath] && ![fileManager fileExistsAtPath:modelParamPath]) {
            break;
        }
        // model 文件地址
        std::string model_path_str = std::string([modelPath UTF8String]);
        // model param 文件地址
        std::string model_param_path_str = std::string([modelParamPath UTF8String]);
        // model内存大小
        size_t model_content_size = 0;
        // model param内存大小
        size_t model_param_content_size = 0;
        
        char *model_content = [self getFileContent:model_path_str.c_str() fileSize:&model_content_size];
        char *model_param_content = [self getFileContent:model_param_path_str.c_str() fileSize:&model_param_content_size];
        
        config.paramSize = model_param_content_size;
        config.paramPointer = model_param_content;
        config.modelSize = model_content_size;
        config.modelPointer = model_content;
        // model_content的内存不能手动释放，否则会导致paddle gpu load 时期（运行）crash,空间开辟在栈上，无需手动释放，系统自动回收
    } while (0);
    return config;
}


- (long)getFileSize:(FILE *)file_handle {
    //获取当前读取文件的位置 进行保存
    long current_read_position = ftell(file_handle);
    long file_size = 0;
    fseek(file_handle, 0, SEEK_END);
    //获取文件的大小
    file_size = ftell(file_handle);
    //恢复文件原来读取的位置
    fseek(file_handle, current_read_position, SEEK_SET);
    return file_size;
}

- (char *)getFileContent:(const char *)fileName fileSize:(size_t *)fileSize {
    // open file
    FILE *aFile = fopen(fileName, "r");
    
    // get file size
    size_t size = [self getFileSize:aFile];
    if (fileSize != nullptr) {
        *fileSize = size;
    }
    
    // get file content
    char *fileContent = (char *)malloc(sizeof(char)*size);
    fread(fileContent, 1, size, aFile);
    
    // flose file
    fclose(aFile);
    
    // return
    return fileContent;
}

- (void)mml_internalPaddleGPUPredictWithTexture:(id<MTLTexture>)modelInputData
                                           dims:(NSArray *)dims
                                 comletionBlock:(MMLMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:MMLMachinePredicateNotSupportSimulator userInfo:nil];
    completionBlock(nil, aError);
    return;
#else
    [self predictWithTexture:modelInputData dims:dims completionBlock:^(PaddleGPUResult * _Nullable gpuPredicateOutputs, NSError * _Nullable error) {
        if (error) {
            if (gpuPredicateOutputs) {
                [gpuPredicateOutputs releaseOutput];
                gpuPredicateOutputs = nil;
            }
            //  predict error, clear old machine,need recreat machine，completely free gpuMachine
            [self releaseMachine];
            completionBlock(nil, error);
            
        } else {
            MMLData *outputData = [MMLOutputConvertor convertPaddleGPUOutputToMMLOutput:gpuPredicateOutputs];
            // 调用方处理output data，需要释放gpuPredicateOutputs
            completionBlock(outputData, nil);
        }
    }];
#endif
}

- (void)mml_internalPaddleGPUPredictWithBuffer:(id<MTLBuffer>)modelInputData
                                          dims:(NSArray *)dims
                                comletionBlock:(MMLMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:MMLMachinePredicateNotSupportSimulator userInfo:nil];
    completionBlock(nil, aError);
    return;
#else
    [self predictWithBuffer:modelInputData dims:dims completionBlock:^(PaddleGPUResult * _Nullable gpuPredicateOutputs, NSError * _Nullable error) {
        if (error) {
            if (gpuPredicateOutputs) {
                [gpuPredicateOutputs releaseOutput];
                gpuPredicateOutputs = nil;
            }
            //  predict error, clear old machine,need recreat machine，completely free gpuMachine
            [self releaseMachine];
            completionBlock(nil, error);
            
        } else {
            MMLData *outputData = [MMLOutputConvertor convertPaddleGPUOutputToMMLOutput:gpuPredicateOutputs];
            // 调用方处理output data，需要释放gpuPredicateOutputs
            completionBlock(outputData, nil);
        }
    }];
#endif
}

- (void)mml_internalPaddleGPUPredictWithModelInputData:(MMLInputMatrix *)modelInputData
                                        comletionBlock:(MMLMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:MMLPaddleMachinePredicateErrorDomain code:MMLMachinePredicateNotSupportSimulator userInfo:nil];
    completionBlock(nil, aError);
    return;
#else
    [self predictWithInputMatrix:modelInputData completionBlock:^(PaddleGPUResult * _Nullable gpuPredicateOutputs, NSError * _Nullable error) {
        if (error) {
            if (gpuPredicateOutputs) {
                [gpuPredicateOutputs releaseOutput];
                gpuPredicateOutputs = nil;
            }
            //  predict error, clear old machine,need recreat machine，completely free gpuMachine
            [self releaseMachine];
            completionBlock(nil, error);
            
        } else {
            MMLData *outputData = [MMLOutputConvertor convertPaddleGPUOutputToMMLOutput:gpuPredicateOutputs];
            // 调用方处理output data，需要释放gpuPredicateOutputs
            completionBlock(outputData, nil);
        }
    }];
#endif
}

#pragma mark - Getter
- (id<MTLDevice>)device {
    if (!_device) {
        _device = MTLCreateSystemDefaultDevice();
    }
    return _device;
}

- (id<MTLCommandQueue>)paddleMTLCommandQueue {
    return [self.device newCommandQueue];
}

@end
