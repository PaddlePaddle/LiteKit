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
#import "LiteKitTask.h"
NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, LiteKitTaskQueueStatus) {
    LiteKitTaskQueueStatusDefault = 0,  // default
    LiteKitTaskQueueStatusFree = 1,     // idle
    LiteKitTaskQueueStatusBusy = 2      // busy
};

typedef NS_ENUM(NSInteger, LiteKitTaskQueueErrorCode) {
    LiteKitTaskQueueParamError = 0,      // param error
    LiteKitTaskQueueNULLMachine          // Machine is null
    
};

/// Queue error domain
FOUNDATION_EXPORT NSString * const LiteKitTaskQueueErrorDomain;

/// task Queue
@interface LiteKitTaskQueue : NSObject

/// Queue state
@property (nonatomic, assign, readonly) LiteKitTaskQueueStatus queueStatus;


/// set Logger class name
/// @param loggerClassName logger class name
- (void)setupLoggerName:(NSString * __nullable)loggerClassName;

/// TaskQueue performance switch
/// @param performanceProfiler enable
- (void)enablePerformanceProfiler:(BOOL)performanceProfiler;



/// batch add task to begin of queue
/// @param tasks tasks
/// @param aError error
- (void)addHighPriorityTasks:(NSArray <LiteKitTask *> *)tasks error:(NSError **)aError;

/// batch add tasks to end of queue
/// @param tasks tasks
/// @param aError error
- (void)addTasks:(NSArray <LiteKitTask *> *)tasks error:(NSError **)aError;

/// batch remove tasks
/// @param tasks tasks
/// @param aError error
- (void)removeTasks:(NSArray <LiteKitTask *> *)tasks error:(NSError **)aError;

/// add task to begin of queue
/// @param task task
/// @param aError error
- (void)addHighPriorityTask:(LiteKitTask *)task error:(NSError **)aError;

/// add task to end of queue
/// @param task task
/// @param aError error
- (void)addTask:(LiteKitTask *)task error:(NSError **)aError;

/// remove task
/// @param task task
/// @param aError error
- (void)removeTask:(LiteKitTask *)task error:(NSError **)aError;

/// quert task
/// @param taskID task id
- (LiteKitTaskStatus)taskStatusByID:(NSString *)taskID;

/// remove all tasks
- (void)removeAllTasks;

/// release Machine
- (void)releaseMachine;
@end

NS_ASSUME_NONNULL_END
