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


#import <Foundation/Foundation.h>
#import "MMLData.h"
#import "MMLTask.h"
NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, MMLTaskQueueStatus) {
    MMLTaskQueueStatusDefault = 0,  // 默认状态
    MMLTaskQueueStatusFree = 1,     // 队列空闲
    MMLTaskQueueStatusBusy = 2      // 队列忙
};

typedef NS_ENUM(NSInteger, MMLTaskQueueErrorCode) {
    MMLTaskQueueParamError = 0,      // 参数错误
    MMLTaskQueueNULLMachine          // Machine为空
    
};

/// 队列错误domain
FOUNDATION_EXPORT NSString * const MMLTaskQueueErrorDomain;

/// 任务队列
@interface MMLTaskQueue : NSObject

/// 队列即时状态
@property (nonatomic, assign, readonly) MMLTaskQueueStatus queueStatus;


/// 设置Logger 的类名
/// @param loggerClassName logger类名
- (void)setupLoggerName:(NSString * __nullable)loggerClassName;

/// 设置是否开启TaskQueue的性能数据收集
/// @param performanceProfiler 是否开启
- (void)enablePerformanceProfiler:(BOOL)performanceProfiler;



/// 批量添加高优任务，加到队首
/// @param tasks 任务组
/// @param aError 错误句柄
- (void)addHighPriorityTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError;

/// 批量添加普通任务，加入队尾
/// @param tasks 任务组
/// @param aError 错误句柄
- (void)addTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError;

/// 批量移除任务
/// @param tasks 任务组
/// @param aError 错误句柄
- (void)removeTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError;

/// 添加高优任务，加到队首
/// @param task 任务
/// @param aError 错误句柄
- (void)addHighPriorityTask:(MMLTask *)task error:(NSError **)aError;

/// 添加普通任务，加入队尾
/// @param task 任务
/// @param aError 错误句柄
- (void)addTask:(MMLTask *)task error:(NSError **)aError;

/// 移除任务
/// @param task 任务
/// @param aError 错误句柄
- (void)removeTask:(MMLTask *)task error:(NSError **)aError;

/// 查询任务状态
/// @param taskID 任务唯一标识
- (MMLTaskStatus)taskStatusByID:(NSString *)taskID;

/// 移除所有的任务
- (void)removeAllTasks;

/// 彻底释放Machine
- (void)releaseMachine;
@end

NS_ASSUME_NONNULL_END
