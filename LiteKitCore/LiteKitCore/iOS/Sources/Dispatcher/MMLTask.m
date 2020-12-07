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


#import "MMLTask.h"
#import "MMLMachineService+Performancer.h"

@interface MMLTask ()

@property (nonatomic, copy, readwrite) NSString *taskID;

@property (nonatomic, strong, readwrite) MMLBaseMachine *machine;

@property (nonatomic, copy, readwrite) MMLTaskCompletionBlock taskBlock;

@property (nonatomic, copy, readwrite) MMLTaskPerformanceBlock taskPerformanceBlock;

@property (nonatomic, strong, readwrite) MMLData *inputData;

@property (nonatomic, assign, readwrite) MMLTaskStatus taskStatus;

@end

@implementation MMLTask

- (instancetype)initTaskWithMachine:(MMLBaseMachine *)machine
                          inputData:(MMLData *)inputData
                    completionBlock:(MMLTaskCompletionBlock)block {
    if (self = [super init]) {
        if (inputData && [inputData isKindOfClass:[MMLData class]]) {
            _inputData = inputData;
        } else {
            return nil;
        }
        
        if (machine && [machine isKindOfClass:[MMLBaseMachine class]]) {
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


- (instancetype)initTaskWithMachine:(MMLBaseMachine *)machine
                          inputData:(MMLData *)inputData
                   performanceBlock:(MMLTaskPerformanceBlock)block {
    if (self = [super init]) {
        if (inputData && [inputData isKindOfClass:[MMLData class]]) {
            _inputData = inputData;
        } else {
            return nil;
        }
        
        if (machine && [machine isKindOfClass:[MMLBaseMachine class]]) {
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
    self.taskStatus = MMLTaskStatusExecuting;
    [self.machine predictWithInputData:self.inputData
                       completionBlock:^(MMLData * _Nullable outputData, NSError * _Nullable error) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            if (self.taskBlock && self.taskStatus == MMLTaskStatusExecuting) {
                self.taskBlock(outputData, error);
            }
            self.taskStatus = MMLTaskStatusFinished;
            
            if (block) {
                block(error);
            }
        });
    }];
}

- (void)runPerformanceTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block {
    self.taskStatus = MMLTaskStatusExecuting;
    [self.machine predictWithInputData:self.inputData
                      performanceBlock:^(MMLData * _Nullable outputData,
                                         MMLPerformanceProfiler * _Nullable performanceProfiler,
                                         NSError * _Nullable error) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            if (self.taskPerformanceBlock && self.taskStatus == MMLTaskStatusExecuting) {
                self.taskPerformanceBlock(outputData, performanceProfiler, error);
            }
            self.taskStatus = MMLTaskStatusFinished;
            
            if (block) {
                block(error);
            }
        });
    }];
}

- (void)cancel {
    self.taskStatus = MMLTaskStatusCanceled;
}


@end
