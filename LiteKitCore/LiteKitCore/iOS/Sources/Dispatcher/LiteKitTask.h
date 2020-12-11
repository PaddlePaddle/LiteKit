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


#import <Foundation/Foundation.h>
#import "LiteKitData.h"
#import "LiteKitBaseMachine.h"
#import "LiteKitPerformanceProfiler.h"

NS_ASSUME_NONNULL_BEGIN

typedef void (^LiteKitTaskCompletionBlock) (LiteKitData * __nullable outputData, NSError * __nullable aError);
typedef void (^LiteKitTaskPerformanceBlock) (LiteKitData * __nullable outputData,
                                         LiteKitPerformanceProfiler * __nullable performanceProfiler,
                                         NSError * __nullable aError);

typedef NS_ENUM(NSInteger, LiteKitTaskStatus) {
    LiteKitTaskStatusWaiting = 0,       // default state，idle
    LiteKitTaskStatusExecuting,         // excuting state，excuting
    LiteKitTaskStatusCanceled,          // cancel state，canceled
    LiteKitTaskStatusFinished           // finish state，execute complete
};

/// task Instance
@interface LiteKitTask : NSObject
/// tasd identifier
@property (nonatomic, copy, readonly) NSString *taskID;
/// machine
@property (nonatomic, strong, readonly) LiteKitBaseMachine *machine;
/// callback block
@property (nonatomic, copy, readonly) LiteKitTaskCompletionBlock taskBlock;
/// callback block with performance
@property (nonatomic, copy, readonly) LiteKitTaskPerformanceBlock taskPerformanceBlock;
/// input data
@property (nonatomic, strong, readonly) LiteKitData *inputData;
/// task state
@property (nonatomic, assign, readonly) LiteKitTaskStatus taskStatus;

/// create task with inputData
/// @param machine machine
/// @param inputData inputdata
/// @param block callback block
- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine inputData:(LiteKitData *)inputData completionBlock:(LiteKitTaskCompletionBlock)block;

///  create performance task with inputData
/// @param machine machine
/// @param inputData inputData
/// @param block performance callback block
- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine inputData:(LiteKitData *)inputData performanceBlock:(LiteKitTaskPerformanceBlock)block;

/// execute Task
/// @param block complete block
- (void)runTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block;

/// execute Task with performance data
/// @param block complete block
- (void)runPerformanceTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block;

/// set status cancel
- (void)cancel;
@end

NS_ASSUME_NONNULL_END
