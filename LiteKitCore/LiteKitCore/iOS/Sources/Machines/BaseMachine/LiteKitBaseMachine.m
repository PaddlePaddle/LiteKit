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


#import "LiteKitBaseMachine.h"
#import "LiteKitBaseMachine+Performancer.h"

// error domain
NSString *const LiteKitMachinInitErrorDomain = @"PaddleMachineInitError";
NSString *const LiteKitMachinPredicateErrorDomain = @"PaddleMachinePredicateError";

static NSInteger const LiteKitBaseMachineLockTime = 10;

@interface LiteKitBaseMachine ()

@property (nonatomic, strong) dispatch_semaphore_t lock;

@property (nonatomic, copy, readwrite) NSString *machineId;

@end

@implementation LiteKitBaseMachine

#pragma mark - Init

- (instancetype)init {
    if (self = [super init]) {
        _lock = dispatch_semaphore_create(0);
    }
    return self;
}

#pragma mark - Predict

/// sync
/// @param aInputData LiteKitData类型的输入数据
/// @param aError 错误信息
- (LiteKitData * __nullable)predictWithInputData:(LiteKitData * __nonnull)aInputData error:(NSError **)aError {
    __block LiteKitData *outData = nil;
    __weak typeof (self) weakSelf = self;
    [self predictWithInputData:aInputData completionBlock:^(LiteKitData * _Nullable outputData, NSError * _Nullable error) {
        if (!error) {
            outData = outputData;
        }
        dispatch_semaphore_signal(weakSelf.lock);
    }];
    dispatch_semaphore_wait(self.lock, dispatch_time(DISPATCH_TIME_NOW, (int64_t)(LiteKitBaseMachineLockTime * NSEC_PER_SEC)));
    return outData;
}

- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData
            performanceBlock:(LiteKitMachinePredictPerformanceBlock)aBlock {
    NSTimeInterval runStart = [[NSDate date] timeIntervalSince1970];
    [self predictWithInputData:aInputData
               completionBlock:^(LiteKitData * _Nullable outputData, NSError * _Nullable error) {
        NSTimeInterval runEnd = [[NSDate date] timeIntervalSince1970];
        LiteKitPerformanceProfiler *profiler = [[LiteKitPerformanceProfiler alloc] init];
        [profiler appendPerformanceData:[NSString stringWithFormat:@"%.3f", (runEnd - runStart) * 1000]
                                    key:LiteKitPerformancePredictTimeForInterface];
        if (!error) {
            [profiler appendPerformanceData:self.predictTime key:LiteKitPerformancePredictTimeForInferenceEngine];
        }
        aBlock(outputData, profiler, error);
    }];
}


/// async
/// @param aInputData LiteKitData类型的输入数据
/// @param aBlock 完成回调
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock {
    return;
}

#pragma mark - Clear

/// 清理machine内存，不释放Machine
- (void)clearMachine {
    return;
}

#pragma mark - Release

/// 释放Machine,置空Machine
- (void)releaseMachine {
    return;
}


#pragma mark - Getter

- (NSString *)machineId {
    if (!_machineId) {
        _machineId = [NSString stringWithFormat:@"machine_%lu", (unsigned long)self.hash];
    }
    
    return _machineId;
}


@end
