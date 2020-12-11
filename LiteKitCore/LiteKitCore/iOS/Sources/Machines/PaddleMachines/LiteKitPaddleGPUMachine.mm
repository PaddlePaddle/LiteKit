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


#import "LiteKitPaddleGPUMachine.h"
#import <Metal/MTLTexture.h>
#import <Metal/Metal.h>
#import "LiteKitBaseMachine+Logger.h"
#import "LiteKitBaseMachine+Performancer.h"

#import "PaddleGPU+MetalResource.h"
#import "LiteKitInputConvertor.h"
#import "LiteKitOutputConvertor.h"

#include <string>
#include <iostream>

/// Paddle GPU Machine
@interface LiteKitPaddleGPUMachine ()
@property (nonatomic, strong) id<MTLDevice> device;
@property (nonatomic, strong) PaddleGPU *paddleGPU;
@property (nonatomic, strong) dispatch_semaphore_t semaphore;
@property (nonatomic, strong) PaddleGPUConfig *gpuConfig;
@end

@implementation LiteKitPaddleGPUMachine

/*
* @brief Action = init paddle GPU Machine
* @param - modelURL model file path
* @param - paramPath params file path
* @param - netType net type
* @param - config origin ModelConfig
* @param - error error message （ErrorDomain:LiteKitPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
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
                        *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitMachineInitFailed userInfo:nil];
                    }
                    return nil;
                }
                
                if (!SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) { // < 9.0
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitIllegalOSVersion userInfo:nil];
                    }
                    return nil;
                }
                
                _gpuConfig = [self litekit_appendConfigWithMetalLibPathIfNeed:_gpuConfig netType:netType error:error];
                if (*error) {
                    return nil;
                }
                
                _paddleGPU = [[PaddleGPU alloc] initWithCommandQueue:[self paddleMTLCommandQueue]
                                                                       net:netType
                                                               modelConfig:_gpuConfig];
                if (!_paddleGPU) {
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitMachineInitFailed userInfo:nil];
                    }
                    return nil;
                }
                
                NSError *aError = nil;
                [self litekit_loadMachine:&aError];
                
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
                    *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitModelFileNotFound userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, model file is not found -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (long)(*error).code, (*error).userInfo]];
                }
            }
        } else {
            //error
            self = nil;
            if (error != NULL) {
                *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitIllegalModelFileURL userInfo:nil];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, model file is error -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (*error).code, (*error).userInfo]];
            }
        }
    }
    
    return self;
}

/**
 init paddle GPU machine

 @param modelConfig model config
 @param netType net type
 @param error error message
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
                    *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitMachineInitFailed userInfo:nil];
                }
                return nil;
            }
            
            if (!SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")) { // < 9.0
                if (error != NULL) {
                    *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitIllegalOSVersion userInfo:nil];
                }
                return nil;
            }
            
            _gpuConfig = [self litekit_appendConfigWithMetalLibPathIfNeed:_gpuConfig netType:netType error:error];
            if (*error) {
                return nil;
            }
            
            _paddleGPU = [[PaddleGPU alloc] initWithCommandQueue:[self paddleMTLCommandQueue]
                                                                   net:netType
                                                           modelConfig:_gpuConfig];
            if (!_paddleGPU) {
                if (error != NULL) {
                    *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitMachineInitFailed userInfo:nil];
                }
                return nil;
            }
            
            NSError *aError = nil;
            [self litekit_loadMachine:&aError];
            
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
                *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitIllegalModelConfig userInfo:nil];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, modelconfig is not correct -- domain:%@, code:%ld, ext:%@", (*error).domain.description, (*error).code, (*error).userInfo]];
            }
        }
    }
    return self;
}

#pragma mark - Super

- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock {
    if (!self) {
        NSError *error = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:LiteKitMachinePredicateMachineNotReady userInfo:nil];
        aBlock(nil, error);
        return;
    }
  
    switch (aInputData.type) {
        case TLiteKitDataTypeMTLTexture:{
            [self litekit_internalPaddleGPUPredictWithTexture:aInputData.mtlTexture dims:aInputData.dims comletionBlock:aBlock];
            break;
        }
            
        case TLiteKitDataTypeMTLBuffer:{
            [self litekit_internalPaddleGPUPredictWithBuffer:aInputData.mtlBuffer dims:aInputData.dims comletionBlock:aBlock];
            break;
        }
            
        default:{
            LiteKitInputMatrix *inputMatrix = [LiteKitInputConvertor convertLiteKitInputToPaddleInput:aInputData];
            [self litekit_internalPaddleGPUPredictWithModelInputData:inputMatrix comletionBlock:aBlock];
            break;
        }
    }
    
}

- (void)releaseMachine {
    [self.paddleGPU clear];
    self.paddleGPU = nil;
}

/**
 clear machine memory
 */
- (void)clearMachine {
    [self.paddleGPU clear];
}


#pragma mark - Public

/**
 predict interface

 @param input input data
 @param dims dims
 @param callback output call back
*/
- (void)predictWithBuffer:(id<MTLBuffer>)input
                     dims:(NSArray *)dims
          completionBlock:(LiteKitPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || [dims count] < 4
        || ![input conformsToProtocol:@protocol(MTLBuffer)] ) {
        NSError *aError = [NSError errorWithDomain:LiteKitMachinPredicateErrorDomain code:LiteKitMachinePredicateInputDataError userInfo:nil];
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
            aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateUpdateDimError userInfo:exception.userInfo];
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
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                 
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // when task end，if call back execute in async thread，will release before callback complete,and lead a crash
                    // so LiteKitShapedData will copy data，lifecycle same as shapedData，here can release immediately
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
             }
        }];
    } else {
        // error model release
        NSError *aError = nil;
        aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}


/**
 predict interface

 @param input input data
 @param callback output call back
*/
- (void)predictWithTexture:(id<MTLTexture>)input
                      dims:(NSArray *)dims
           completionBlock:(LiteKitPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || ![input conformsToProtocol:@protocol(MTLTexture)] ) {
        NSError *aError = [NSError errorWithDomain:LiteKitMachinPredicateErrorDomain code:LiteKitMachinePredicateInputDataError userInfo:nil];
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
            aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateUpdateDimError userInfo:exception.userInfo];
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
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // when task end，if call back execute in async thread，will release before callback complete,and lead a crash
                    // so LiteKitShapedData will copy data，lifecycle same as shapedData，here can release immediately
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
            }
        }];
    } else {
        // error machine already release
        NSError *aError = nil;
        aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}

/**
predict interface

@param input input data
@param callback output call back
*/
- (void)predictWithInputMatrix:(LiteKitInputMatrix *)input
               completionBlock:(LiteKitPaddleGPUPredicatOutputsCallback)callback {
    if (!input
        || ![input isKindOfClass:[LiteKitInputMatrix class]]
        || input.pixels == NULL
        || input.height <= 0
        || input.width <= 0) {
        NSError *aError = [NSError errorWithDomain:LiteKitMachinPredicateErrorDomain code:LiteKitMachinePredicateInputDataError userInfo:nil];
        callback(nil, aError);
        return;
    }
    
    if (self.paddleGPU) {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        // dims is n h w c，memory\input\output rank are all nchw，so input.pixels rank nchw
        // TODO:gpu will not support getTexture API，use predictWithBuffer API immediately(PaddleGPU.m should suupport the interface)
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
            aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateUpdateDimError userInfo:exception.userInfo];
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
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    callback(nil, aError);
                    return;
                }
                PaddleGPUResult *gpuResult = [result firstObject];
                
                if (gpuResult && gpuResult.output && gpuResult.outputSize > 0) {
                    [self.logger debugLogMsg:@"paddle gpu -- predict success"];
                    callback(gpuResult, nil);
                    // when task end，if call back execute in async thread，will release before callback complete,and lead a crash
                    // so LiteKitShapedData will copy data，lifecycle same as shapedData，here can release immediately
                    [gpuResult releaseOutput];
                } else {
                    // release gpuResult
                    [gpuResult releaseOutput];
                    aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicatePredictError userInfo:nil];
                    [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, predict error -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
                    callback(nil, aError);
                }
            }
        }];
        // release buffer
        buffer = nil;
    } else {
        // error machine already release
        NSError *aError = nil;
        aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateDestroyed userInfo:nil];
        callback(nil, aError);
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, paddleGPU is nil -- domain:%@, code:%ld, ext:%@", (aError).domain.description, (aError).code, (aError).userInfo]];
    }
}

/**
  release GPU and clear machine,free memory
 */
- (void)releasePaddleGPU {
    [self.paddleGPU clear];
    self.paddleGPU = nil;
}

/**
 reload machine
 */
- (void)reloadMachineWithError:(NSError **)error {
    if (self.paddleGPU) {
        [self litekit_loadMachine:error];
    } else {
        *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachineLoadFailed userInfo:nil];
    }
}


#pragma mark - Private

/**
 load paddle gpu

 @param aError paddle gpu load result
 */
- (void)litekit_loadMachine:(NSError **)aError {
    @try {
        NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
        [_paddleGPU loadWithError:aError];
        NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
        [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu load time: %f ms", (end - start) * 1000]];
        self.loadTime = [NSString stringWithFormat:@"%.3f", (end - start) * 1000];
    } @catch (NSException *exception) {
        *aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitMachinePredicateLoadError userInfo:@{NSLocalizedDescriptionKey:exception.description}];
        [self.logger errorLogMsg:[NSString stringWithFormat:@"paddle gpu, machine load error -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
    }
}

/**
 metallib config
 @param config init config
 @param netType net type
 @param error error
 @return ModelConfig instance
 */
- (PaddleGPUConfig *)litekit_appendConfigWithMetalLibPathIfNeed:(PaddleGPUConfig *)config
                                                netType:(NetType)netType
                                                  error:(NSError **)error {
    do {
        if (!config || ![config isKindOfClass:[PaddleGPUConfig class]]) {
            break;
        }
        // already set customm metal path，will not load default metal path
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
                *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachinePredicateErrorCode)LiteKitIllegalModelConfig userInfo:@{aError.domain:codeNumber}];
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
 load a new GPU Config
 
 @return config instance
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
        // model file path path
        std::string model_path_str = std::string([modelPath UTF8String]);
        // model param file path
        std::string model_param_path_str = std::string([modelParamPath UTF8String]);
        // model size
        size_t model_content_size = 0;
        // model param size
        size_t model_param_content_size = 0;
        
        char *model_content = [self getFileContent:model_path_str.c_str() fileSize:&model_content_size];
        char *model_param_content = [self getFileContent:model_param_path_str.c_str() fileSize:&model_param_content_size];
        
        config.paramSize = model_param_content_size;
        config.paramPointer = model_param_content;
        config.modelSize = model_content_size;
        config.modelPointer = model_content;
        // memory of model_content will not release manual，or will lead a paddle gpu load period（running）crash,space is on heap，no need release manual，system recovery
    } while (0);
    return config;
}


- (long)getFileSize:(FILE *)file_handle {
    //get file read position
    long current_read_position = ftell(file_handle);
    long file_size = 0;
    fseek(file_handle, 0, SEEK_END);
    // get file size
    file_size = ftell(file_handle);
    //reset file read position
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

- (void)litekit_internalPaddleGPUPredictWithTexture:(id<MTLTexture>)modelInputData
                                           dims:(NSArray *)dims
                                 comletionBlock:(LiteKitMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:LiteKitMachinePredicateNotSupportSimulator userInfo:nil];
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
            LiteKitData *outputData = [LiteKitOutputConvertor convertPaddleGPUOutputToLiteKitOutput:gpuPredicateOutputs];
            // process output data，need to release gpuPredicateOutputs
            completionBlock(outputData, nil);
        }
    }];
#endif
}

- (void)litekit_internalPaddleGPUPredictWithBuffer:(id<MTLBuffer>)modelInputData
                                          dims:(NSArray *)dims
                                comletionBlock:(LiteKitMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:LiteKitMachinePredicateNotSupportSimulator userInfo:nil];
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
            LiteKitData *outputData = [LiteKitOutputConvertor convertPaddleGPUOutputToLiteKitOutput:gpuPredicateOutputs];
            // process output data，need to gpuPredicateOutputs
            completionBlock(outputData, nil);
        }
    }];
#endif
}

- (void)litekit_internalPaddleGPUPredictWithModelInputData:(LiteKitInputMatrix *)modelInputData
                                        comletionBlock:(LiteKitMachinePredictCompletionBlock)completionBlock {
#if TARGET_IPHONE_SIMULATOR
    NSError *aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:LiteKitMachinePredicateNotSupportSimulator userInfo:nil];
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
            LiteKitData *outputData = [LiteKitOutputConvertor convertPaddleGPUOutputToLiteKitOutput:gpuPredicateOutputs];
            // process output data，need to gpuPredicateOutputs
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
