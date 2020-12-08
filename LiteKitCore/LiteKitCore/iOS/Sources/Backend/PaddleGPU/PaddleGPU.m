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


#import "PaddleGPU.h"

#import <Foundation/Foundation.h>
#if TARGET_IPHONE_SIMULATOR
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
#import <paddle_mobile/paddle_mobile.h>
#import <paddle_mobile/paddle_mobile-Swift.h>
#endif
#ifndef BBALogInfo
#define BBALogInfo(frmt, ...)
#else
#import <BBALumberjack/BBALumberjack.h>
#endif

#endif


@implementation PaddleGPUConfig
#pragma mark - Init
- (instancetype)init {
    if (self = [super init]) {
        self.computePrecision = PrecisionTypeFloat16;
        self.modelPrecision = PrecisionTypeFloat32;
        self.metalLoadType =  LoadMetalInPaddleGPU;
        self.useMPS = NO;
        self.enableDebug = NO;
    }
    return self;
}

@end

@interface PaddleGPUResult ()
#if TARGET_IPHONE_SIMULATOR
- (void)setOutputResult:(id)resultHolder;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
@property (strong, nonatomic) ResultHolder *resultHolder;

- (void)setOutputResult:(ResultHolder *)resultHolder;
#endif
#endif
@end

@implementation PaddleGPUResult
#if TARGET_IPHONE_SIMULATOR
-(void)releaseOutput {
    // do nothing
}
- (void)setOutputResult:(id)resultHolder {
    // do nothing
}

#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
- (void)setOutputResult:(ResultHolder *)resultHolder {
    self.resultHolder = resultHolder;
    self.output = resultHolder.result;
    self.outputSize = (int)resultHolder.capacity;
}

-(void)releaseOutput {
    [self.resultHolder releasePointer];
}
#endif

#endif
@end

@interface PaddleGPU () {
#if TARGET_IPHONE_SIMULATOR
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
  Runner *runner;
#endif
#endif
}
@end

@implementation PaddleGPU
#pragma mark - Init
-(instancetype)initWithCommandQueue:(id<MTLCommandQueue>)queue net:(NetType)netType modelConfig:(PaddleGPUConfig *)config {
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
#if TARGET_IPHONE_SIMULATOR
    return nil;
#else
    
    self = [super init];
#ifdef DEBUG
    if (config.enableDebug) {
        BBALogInfo(@" init with command queue %s", __func__);
        BBALogInfo(@"start ---  config.paramSize == %d, config.modelSize == %d ,config.modelPrecision == %lu,config.computePrecision == %lu,config.useMPS == %d", config.paramSize, config.modelSize, (unsigned long)config.modelPrecision, (unsigned long)config.computePrecision, config.useMPS);
    }
#endif
    if (self) {
        
        if (config == nil || ![config isKindOfClass:[PaddleGPUConfig class]]) {
            return nil;
        }
        
        Net *net = nil;
        NSError *error = nil;
        if (netType == SuperResolutionNetType) {
            // memory load way
            net = [[SuperResolutionNet alloc] initWithDevice:queue.device
                                              inParamPointer:config.paramPointer
                                                 inParamSize:config.paramSize
                                              inModelPointer:config.modelPointer
                                                 inModelSize:config.modelSize
                                                       error:&error];
        } else if ( netType == MobileNetCombileType) {
            net = [[MobileNetCombined alloc] initWithDevice:queue.device
                                             inParamPointer:config.paramPointer
                                                inParamSize:config.paramSize
                                             inModelPointer:config.modelPointer
                                                inModelSize:config.modelSize
                                                      error:&error];
            //    [runner updateInputDimInDim:config.dims];
        } else if (netType == YoloNetType) {
            net = [[YoloNet alloc] initWithDevice:queue.device
                                   inParamPointer:config.paramPointer
                                      inParamSize:config.paramSize
                                   inModelPointer:config.modelPointer
                                      inModelSize:config.modelSize
                                            error:&error];
        } else if (netType == CustomNetType) {
            net = [[Net alloc] initWithDevice:queue.device
                               inParamPointer:config.paramPointer
                                  inParamSize:config.paramSize
                               inModelPointer:config.modelPointer
                                  inModelSize:config.modelSize
                                        error:&error];
        }
        
        if (net && !error) {
            // 是否开启MPS
            net.useMPS = config.useMPS;
            // 是否开启最高等级的加速策略
            net.useAggressiveOptimization = config.useAggressiveOptimization;
            // 模型精度
            if (config.modelPrecision == PrecisionTypeFloat16) {
                net.paramPrecision = PrecisionFloat16;
            } else {
                net.paramPrecision = PrecisionFloat32;
            }
            // 运算精度
            GlobalConfig *globalConfig = [GlobalConfig shared];
            if (config.computePrecision == PrecisionTypeFloat16) {
                globalConfig.computePrecision = PrecisionFloat16;
            } else {
                globalConfig.computePrecision = PrecisionFloat32;
            }
            
            // 是否开启日志
            globalConfig.debug = config.enableDebug;
            /// 于PaddleGPU而言:
            /// LoadMetalInCustomMetalLib 才是外界设置的自定义的路径，
            /// LoadMetalInPaddle   为各个Net封装的
            /// 当 metalLoadType 为 LoadMetalInCustomMetalLib 时， metal library 路径不能为空
            net.metalLoadMode = config.metalLoadType;
            if (config.metalLoadType == LoadMetalInCustomMetalLib) {
                net.metalLibPath = config.metalLibPath;
            }
            // 设置dims
            if (config.dims && [config.dims count] > 0 && [config.dims isKindOfClass:[NSArray class]]) {
                net.inputDim = [[Dim alloc] initInDim:config.dims];
            }
#ifdef DEBUG
            if (config.enableDebug) {
                BBALogInfo(@"end ---- net.paramSize == %ld, net.modelSize == %ld ,net.modelPrecision == %ld,globalConfig.computePrecision == %ld,metalLoadType == %lu, net.useMPS == %d", (long)net.paramSize, (long)net.modelSize, (long)net.paramPrecision, (long)globalConfig.computePrecision, (unsigned long)config.metalLoadType, net.useMPS);
            }
#endif
            // PaddleGPU对超分定制的优化
            if (netType == SuperResolutionNetType) {
                net = [self openAggressiveOptimizationWithNet:net];
            }
        } else {
            return nil;
        }
        
        runner = [[Runner alloc] initInNet:net commandQueue:queue error:&error];
        if (error || !runner) {
            return nil;
        }
    }
    return self;
#endif
#else
    return nil;
#endif
}

#if TARGET_IPHONE_SIMULATOR
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
// 开启Aggressive优化
- (Net *)openAggressiveOptimizationWithNet:(Net *)net {
    net.useAggressiveOptimization = YES;
    return net;
}
#endif
#endif


- (BOOL)load {
#if TARGET_IPHONE_SIMULATOR
    return NO;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    NSError *error = nil;
    [runner loadAndReturnError:&error];
    if (error) {
        return NO;
    } else {
        return YES;
    }
#endif
#endif
  
}

/**
 待error信息的load

 @param error error信息
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error {
#if TARGET_IPHONE_SIMULATOR
    *error = [NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"simulator not supported"}];
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [runner loadAndReturnError:error];
#endif
#endif
}

- (BOOL)updateInputDim:(NSArray <NSNumber *>*)dims {
#if TARGET_IPHONE_SIMULATOR
    return NO;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    Dim *inputDim = [[Dim alloc] initInDim:dims];
    NSError *error = nil;
    [runner updateInputDimInDim:inputDim error:&error];
    if (error) {
        return NO;
    } else {
        return YES;
    }
#endif
#endif
}

/**
 带error信息的updateInputDims

 @param dims 待更新的dims
 @param error error信息
 */
-(void)updateInputDimInDim:(NSArray <NSNumber *>*)dims error:(NSError *__autoreleasing  _Nullable * _Nullable)error {
#if TARGET_IPHONE_SIMULATOR
    *error = [NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"simulator not supported"}];
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    Dim *inputDim = [[Dim alloc] initInDim:dims];
    [runner updateInputDimInDim:inputDim error:error];
#endif
#endif
}

- (void)getTextureWithImage:(CGImageRef)image getTexture:(void (^) (id<MTLTexture>))block {
#if TARGET_IPHONE_SIMULATOR
    block(nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [runner getTextureWithImage:image getTexture:^(BOOL success, id<MTLTexture> _Nullable texture) {
        if (success) {
            block(texture);
        } else {
            block(nil);
        }
    }];
#endif
#endif
    
}


- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block {
#if TARGET_IPHONE_SIMULATOR
    block(nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [runner getTextureInBuffer:buffer getTexture:^(BOOL success, id<MTLTexture> _Nullable texture) {
        if (success) {
            block(texture);
        } else {
            block(nil);
        }
    } channelNum:1];
#endif
#endif
    
    
}

- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block channelNum:(NSInteger)channelNum {
#if TARGET_IPHONE_SIMULATOR
    block(nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [runner getTextureInBuffer:buffer getTexture:^(BOOL success, id<MTLTexture> _Nullable texture) {
        if (success) {
            block(texture);
        } else {
            block(nil);
        }
    } channelNum:channelNum];
#endif
#endif
    
    
}


- (void)predict:(id<MTLTexture>)texture withCompletion:(void (^)(BOOL, NSArray<NSArray <NSNumber *>*> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion(NO,nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [self predict:texture withErrorCompletion:^(NSError * _Nullable error, NSArray<NSArray<NSNumber *> *> *resultArr) {
        if (error) {
            completion(NO,nil);
        } else {
            completion(YES,resultArr);
        }
    }];
#endif
#endif
}


/**
 带error信息的预测，output为原始数据

 @param texture texture
 @param completion 回调
 */
-(void)predict:(id<MTLTexture>)texture withErrorCompletion:(void (^)(NSError * _Nullable, NSArray<NSArray <NSNumber *>*> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"simulator not supported"}],nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    if (!texture) {
        dispatch_async(dispatch_get_main_queue(), ^{
            completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"texture nil"}],nil);
        });
        return;
    }
    [runner predictWithTexture:texture completion:^(NSError * _Nullable error, NSArray<ResultHolder *> * _Nullable resultArr) {
        if (error) {
            completion(error, nil);
        } else {
            NSMutableArray<NSMutableArray <NSNumber *>*> *ocResultArray = [NSMutableArray arrayWithCapacity:resultArr.count];
            for (int i = 0; i < resultArr.count; ++i) {
                ResultHolder *resultHolder = resultArr[i];
                NSMutableArray <NSNumber *>*res = [NSMutableArray arrayWithCapacity:resultHolder.capacity];
                for (int j = 0; j < resultHolder.capacity; ++j) {
                    [res addObject:[NSNumber numberWithFloat:resultHolder.result[j]]];
                }
                [ocResultArray addObject:res];
                [resultHolder releasePointer];
            }
            completion(nil, ocResultArray);
        }
    }];
#endif
#endif
}

- (void)predict:(id<MTLTexture>)texture withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion(NO,nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [self predict:texture withErrorResultCompletion:^(NSError * _Nullable error, NSArray<PaddleGPUResult *> *gpuResult) {
        if (error) {
            completion(NO,nil);
        } else {
            completion(YES,gpuResult);
        }
    }];
#endif
#endif
 
}

/**
 带error信息的预测，output封装成PaddleGPUResult
 
 @param texture 需要进行预测的图像转换的 texture
 @param completion 预测完成回调
 */
-(void)predict:(id<MTLTexture>)texture withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"simulator not supported"}],nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    if (!texture) {
        dispatch_async(dispatch_get_main_queue(), ^{
            completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"texture nil"}],nil);
        });
        return;
    }
    [runner predictWithTexture:texture completion:^(NSError * _Nullable error, NSArray<ResultHolder *> * _Nullable resultArr) {
        if (error) {
            completion(error, nil);
        } else {
            NSMutableArray <PaddleGPUResult *> *ocResultArr = [NSMutableArray arrayWithCapacity:resultArr.count];
            for (int i = 0; i < resultArr.count; ++i) {
                ResultHolder *result = resultArr[i];
                PaddleGPUResult *gpuResult = [[PaddleGPUResult alloc] init];
                gpuResult.dim = result.dim;
                [gpuResult setOutputResult:result];
                [ocResultArr addObject:gpuResult];
            }
            completion(nil, ocResultArr);
        }
    }];
#endif
#endif
}

- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion(NO,nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [self predictWithBuffer:buffer channelNum:channelNum withErrorResultCompletion:^(NSError * _Nullable error, NSArray<PaddleGPUResult *> *gpuResult) {
        if (error) {
            completion(NO,nil);
        } else {
            completion(YES,gpuResult);
        }
    }];
#endif
#endif
 
}


- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion {
#if TARGET_IPHONE_SIMULATOR
    completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"simulator not supported"}],nil);
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    if (!buffer) {
        dispatch_async(dispatch_get_main_queue(), ^{
            completion([NSError errorWithDomain:NSCocoaErrorDomain code:0 userInfo:@{@"msg": @"buffer nil"}],nil);
        });
        return;
    }
    [runner predictWithBuffer:buffer channelNum:channelNum completion:^(NSError * _Nullable error, NSArray<ResultHolder *> * _Nullable resultArr) {
        if (error) {
            completion(error, nil);
        } else {
            NSMutableArray <PaddleGPUResult *> *ocResultArr = [NSMutableArray arrayWithCapacity:resultArr.count];
            for (int i = 0; i < resultArr.count; ++i) {
                ResultHolder *result = resultArr[i];
                PaddleGPUResult *gpuResult = [[PaddleGPUResult alloc] init];
                gpuResult.dim = result.dim;
                [gpuResult setOutputResult:result];
                [ocResultArr addObject:gpuResult];
            }
            completion(nil, ocResultArr);
        }
    }];
#endif
#endif
}


- (void)clear {
#if TARGET_IPHONE_SIMULATOR
    return;
#else
#if __has_include(<paddle_mobile/paddle_mobile-Swift.h>)
    [runner clear];
#endif
#endif
  
}

@end
