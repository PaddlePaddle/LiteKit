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
