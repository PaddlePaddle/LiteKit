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


#import "MMLBaseMachine.h"
#import "MMLBaseMachine+Performancer.h"

// error domain
NSString *const MMLMachinInitErrorDomain = @"PaddleMachineInitError";
NSString *const MMLMachinPredicateErrorDomain = @"PaddleMachinePredicateError";

static NSInteger const MMLBaseMachineLockTime = 10;

@interface MMLBaseMachine ()

@property (nonatomic, strong) dispatch_semaphore_t lock;

@property (nonatomic, copy, readwrite) NSString *machineId;

@end

@implementation MMLBaseMachine

#pragma mark - Init

- (instancetype)init {
    if (self = [super init]) {
        _lock = dispatch_semaphore_create(0);
    }
    return self;
}

#pragma mark - Predict

/// sync
/// @param aInputData MMLData类型的输入数据
/// @param aError 错误信息
- (MMLData * __nullable)predictWithInputData:(MMLData * __nonnull)aInputData error:(NSError **)aError {
    __block MMLData *outData = nil;
    __weak typeof (self) weakSelf = self;
    [self predictWithInputData:aInputData completionBlock:^(MMLData * _Nullable outputData, NSError * _Nullable error) {
        if (!error) {
            outData = outputData;
        }
        dispatch_semaphore_signal(weakSelf.lock);
    }];
    dispatch_semaphore_wait(self.lock, dispatch_time(DISPATCH_TIME_NOW, (int64_t)(MMLBaseMachineLockTime * NSEC_PER_SEC)));
    return outData;
}

- (void)predictWithInputData:(MMLData * __nonnull)aInputData
            performanceBlock:(MMLMachinePredictPerformanceBlock)aBlock {
    NSTimeInterval runStart = [[NSDate date] timeIntervalSince1970];
    [self predictWithInputData:aInputData
               completionBlock:^(MMLData * _Nullable outputData, NSError * _Nullable error) {
        NSTimeInterval runEnd = [[NSDate date] timeIntervalSince1970];
        MMLPerformanceProfiler *profiler = [[MMLPerformanceProfiler alloc] init];
        [profiler appendPerformanceData:[NSString stringWithFormat:@"%.3f", (runEnd - runStart) * 1000]
                                    key:MMLPerformancePredictTimeForInterface];
        if (!error) {
            [profiler appendPerformanceData:self.predictTime key:MMLPerformancePredictTimeForInferenceEngine];
        }
        aBlock(outputData, profiler, error);
    }];
}


/// async
/// @param aInputData MMLData类型的输入数据
/// @param aBlock 完成回调
- (void)predictWithInputData:(MMLData * __nonnull)aInputData completionBlock:(MMLMachinePredictCompletionBlock)aBlock {
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
