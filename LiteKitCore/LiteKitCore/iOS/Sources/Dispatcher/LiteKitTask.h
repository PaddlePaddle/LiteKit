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
    LiteKitTaskStatusWaiting = 0,       // 默认状态，正在等待中
    LiteKitTaskStatusExecuting,         // 执行状态，正在执行中
    LiteKitTaskStatusCanceled,          // 取消状态，已经被取消
    LiteKitTaskStatusFinished           // 完成状态，已经执行完成
};

/// 任务封装实体
@interface LiteKitTask : NSObject
/// 任务标识
@property (nonatomic, copy, readonly) NSString *taskID;
/// 对应的machine
@property (nonatomic, strong, readonly) LiteKitBaseMachine *machine;
/// 结果回调block
@property (nonatomic, copy, readonly) LiteKitTaskCompletionBlock taskBlock;
/// 带performance的结果回调block
@property (nonatomic, copy, readonly) LiteKitTaskPerformanceBlock taskPerformanceBlock;
/// 输入数据
@property (nonatomic, strong, readonly) LiteKitData *inputData;
/// 任务的执行状态
@property (nonatomic, assign, readonly) LiteKitTaskStatus taskStatus;

/// 根据inputData创建任务
/// @param machine machine
/// @param inputData 输入数据
/// @param block 回调block
- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine inputData:(LiteKitData *)inputData completionBlock:(LiteKitTaskCompletionBlock)block;

/// 根据inputData创建带performance的任务
/// @param machine machine
/// @param inputData 输入数据
/// @param block performance回调block
- (instancetype)initTaskWithMachine:(LiteKitBaseMachine *)machine inputData:(LiteKitData *)inputData performanceBlock:(LiteKitTaskPerformanceBlock)block;

/// 运行Task
/// @param block 任务是否完成回调
- (void)runTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block;

/// 运行带性能数据的Task
/// @param block 任务是否完成回调
- (void)runPerformanceTaskWithCompletionBlock:(void(^)(NSError * __nullable error))block;

/// 设置status的状态为取消状态
- (void)cancel;
@end

NS_ASSUME_NONNULL_END
