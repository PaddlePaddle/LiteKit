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
