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


#import "LiteKitTask.h"
#import "LiteKitMachineService+Performancer.h"

@interface LiteKitTask ()

@property (nonatomic, copy, readwrite) NSString *taskID;

@property (nonatomic, strong, readwrite) LiteKitBaseMachine *machine;

@property (nonatomic, copy, readwrite) LiteKitTaskCompletionBlock taskBlock;

@property (nonatomic, copy, readwrite) LiteKitTaskPerformanceBlock taskPerformanceBlock;

@property (nonatomic, strong, readwrite) LiteKitData *inputData;

@property (nonatomic, assign, readwrite) LiteKitTaskStatus taskStatus;

@end

@implementation LiteKitTask

- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine
                          inputData:(LiteKitData *)inputData
                    completionBlock:(LiteKitTaskCompletionBlock)block {
    if (self = [super init]) {
        if (inputData && [inputData isKindOfClass:[LiteKitData class]]) {
            _inputData = inputData;
        } else {
            return nil;
        }
        
        if (machine && [machine isKindOfClass:[LiteKitBaseMachine class]]) {
            _machine = machine;
        } else {
            return nil;
        }
        
        if (block) {
            _taskBlock = block;
        } else {
            return nil;
        }
    }
    return self;
}


- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine
                          inputData:(LiteKitData *)inputData
                   performanceBlock:(LiteKitTaskPerformanceBlock)block {
    if (self = [super init]) {
        if (inputData && [inputData isKindOfClass:[LiteKitData class]]) {
            _inputData = inputData;
        } else {
            return nil;
        }
        
        if (machine && [machine isKindOfClass:[LiteKitBaseMachine class]]) {
            _machine = machine;
        } else {
            return nil;
        }
        
        if (block) {
            _taskPerformanceBlock = block;
        } else {
            return nil;
        }
    }
    return self;
}

- (NSString *)taskID {
    if (!_taskID) {
        if (self.inputData) {
            NSTimeInterval time = [[NSDate date] timeIntervalSince1970];
            _taskID = [NSString stringWithFormat:@"task_%lu_%f", (unsigned long)self.inputData.hash, time];
        } else {
            _taskID = nil;
        }
    }
    return _taskID;
}


#pragma mark - Public

- (void)runTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block {
    self.taskStatus = LiteKitTaskStatusExecuting;
    [self.machine predictWithInputData:self.inputData
                       completionBlock:^(LiteKitData * _Nullable outputData, NSError * _Nullable error) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            if (self.taskBlock && self.taskStatus == LiteKitTaskStatusExecuting) {
                self.taskBlock(outputData, error);
            }
            self.taskStatus = LiteKitTaskStatusFinished;
            
            if (block) {
                block(error);
            }
        });
    }];
}

- (void)runPerformanceTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block {
    self.taskStatus = LiteKitTaskStatusExecuting;
    [self.machine predictWithInputData:self.inputData
                      performanceBlock:^(LiteKitData * _Nullable outputData,
                                         LiteKitPerformanceProfiler * _Nullable performanceProfiler,
                                         NSError * _Nullable error) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            if (self.taskPerformanceBlock && self.taskStatus == LiteKitTaskStatusExecuting) {
                self.taskPerformanceBlock(outputData, performanceProfiler, error);
            }
            self.taskStatus = LiteKitTaskStatusFinished;
            
            if (block) {
                block(error);
            }
        });
    }];
}

- (void)cancel {
    self.taskStatus = LiteKitTaskStatusCanceled;
}


@end
