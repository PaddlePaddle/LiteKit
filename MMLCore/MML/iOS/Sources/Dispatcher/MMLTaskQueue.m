//
//  MMLTaskQueue.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/21.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLTaskQueue.h"
#import "MMLCommonDefines.h"

NSString * const MMLTaskQueueErrorDomain = @"MMLTaskQueueErrorDomain";
static NSString * const kMMLTaskSerialQueueName = @"__kMMLTaskSerialQueueName__";

@interface MMLTaskQueue ()
/// 任务队列
@property (nonatomic, strong) NSMutableArray <MMLTask *> *taskStack;
/// 任务队列中所有任务的Machine
@property (nonatomic, strong) NSMutableDictionary <NSString *, MMLBaseMachine *> *queueMachines;
/// 在单独的线程中执行任务
@property (nonatomic, strong) dispatch_queue_t serial_queue;
/// 日志器
@property (nonatomic, strong) id <MMLLoggerProtocol> logger;
/// 是否打开性能数据统计
@property (nonatomic, assign) BOOL openPerformanceProfiler;
/// 队列状态
@property (nonatomic, assign, readwrite) MMLTaskQueueStatus queueStatus;
@end

@implementation MMLTaskQueue

#pragma mark - Init

- (instancetype)init {
    if (self = [super init]) {
        _serial_queue = dispatch_queue_create(kMMLTaskSerialQueueName.UTF8String, DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

#pragma mark - Dealloc

- (void)dealloc {
    [self mml_releaseMachineInQueueMachines];
    self.queueMachines = nil;
    self.taskStack = nil;
    self.serial_queue = nil;
    [self.logger warningLogMsg:@"Task Queue dealloc"];
    self.logger = nil;
}

#pragma mark - Public

- (void)setupLoggerName:(NSString *)loggerClassName {
    if ([loggerClassName isKindOfClass:[NSString class]] &&
        loggerClassName.length > 0 &&
        NSClassFromString(loggerClassName) &&
        [NSClassFromString(loggerClassName) instancesRespondToSelector:@selector(setLogTag:)]) {
        self.logger = [NSClassFromString(loggerClassName) new];
        [self.logger setLogTag:MMLTaskQueueLoggerTag];
    } else {
#if __has_include("MMLLogger.h")
        self.logger = [[MMLLogger alloc] initWithTag:MMLTaskQueueLoggerTag];
#endif
    }
}

- (void)enablePerformanceProfiler:(BOOL)performanceProfiler {
    self.openPerformanceProfiler = performanceProfiler;
}

- (void)addHighPriorityTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError {
    [self mml_internalBatchAddTasks:tasks atHead:YES error:aError];
}

- (void)addTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError {
    [self mml_internalBatchAddTasks:tasks atHead:NO error:aError];
}

- (void)removeTasks:(NSArray <MMLTask *> *)tasks error:(NSError **)aError {
    if (tasks && [tasks isKindOfClass:[NSArray class]] && tasks.count > 0) {
       dispatch_async(self.serial_queue, ^{
           NSMutableArray *taskStackCopy = [self.taskStack mutableCopy];
           // 循环遍历找出待删除的元素的index
           for (int index = 0; index <= tasks.count - 1; index++) {
               for (int stackIndex = 0; stackIndex < self.taskStack.count; stackIndex++) {
                   MMLTask *taskInQueue = [self.taskStack objectAtIndex:stackIndex];
                   if ([taskInQueue.taskID isEqualToString:tasks[index].taskID]) {
                       [taskInQueue cancel];
                       [taskStackCopy removeObjectAtIndex:index];
                       [self mml_removeMachineByTask:tasks[index]];
                   }
               }
           }
           self.taskStack = [taskStackCopy mutableCopy];
           [self mml_performTaskQueue];
       });
   } else {
       if (aError != NULL) {
           *aError = [NSError errorWithDomain:MMLTaskQueueErrorDomain code:MMLTaskQueueParamError userInfo:nil];
           [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
       }
   }
}

//加到队首
- (void)addHighPriorityTask:(MMLTask *)task error:(NSError **)aError {
    [self mml_internalAddTask:task atHead:YES error:aError];
}

//加入队尾
- (void)addTask:(MMLTask *)task error:(NSError **)aError {
    [self mml_internalAddTask:task atHead:NO error:aError];
}

// 移除单个任务
- (void)removeTask:(MMLTask *)task error:(NSError **)aError {
    if (task && [task isKindOfClass:[MMLTask class]]) {
        dispatch_async(self.serial_queue, ^{
            NSMutableArray *taskStackCopy = [self.taskStack mutableCopy];
            for (int index = 0; index < self.taskStack.count; index++) {
                MMLTask *taskInQueue = [self.taskStack objectAtIndex:index];
                if ([taskInQueue.taskID isEqualToString:task.taskID]) {
                    [taskInQueue cancel];
                    [taskStackCopy removeObjectAtIndex:index];
                }
            }
            self.taskStack = [taskStackCopy mutableCopy];
            
            [self mml_removeMachineByTask:task];
            [self mml_performTaskQueue];
        });
    } else {
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLTaskQueueErrorDomain code:MMLTaskQueueParamError userInfo:nil];
            [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
        }
    }
}

// 查询任务状态
- (MMLTaskStatus)taskStatusByID:(NSString *)taskID {
    MMLTaskStatus status = MMLTaskStatusWaiting;
    for (MMLTask *task in self.taskStack) {
        if ([task.taskID isEqualToString:taskID]) {
            status = task.taskStatus;
            break;
        }
    }
    return status;
}

// 移除所有任务
- (void)removeAllTasks {
    dispatch_async(self.serial_queue, ^{
        [self.taskStack removeAllObjects];
    });
}

/// 释放Machine
- (void)releaseMachine {
    dispatch_async(self.serial_queue, ^{
        [self mml_releaseMachineInQueueMachines];
    });
}

#pragma mark - Task Scheduling

- (void)mml_performTaskQueue {
    if (self.queueStatus == MMLTaskQueueStatusBusy) {
        return;
    } else {
        if (self.taskStack.count > 0) {
            self.queueStatus = MMLTaskQueueStatusBusy;
            [self.logger debugLogMsg:@"队列调度开始"];
            // 先进先出
            MMLTask *nextTask = [self.taskStack firstObject];
            if (nextTask.taskStatus == MMLTaskStatusFinished ||
                nextTask.taskStatus == MMLTaskStatusCanceled) {
                // 出队
                [self mml_taskFinished:nextTask];
            } else if (nextTask.taskStatus == MMLTaskStatusExecuting) { // 任务正在执行
                return;
            } else {
                [self mml_executeTask:nextTask];
            }
        } else {
            // 队列空闲需要手动clearMachine
            [self mml_clearMachineInQueueMachines];
        }
    }
}

- (void)mml_executeTask:(MMLTask *)task {
    [self.logger debugLogMsg:@"execute task start"];
    MMLBaseMachine *taskMachine = task.machine;
    if (taskMachine) {
        // 执行task
        [self mml_executeTaskWithMachine:taskMachine task:task];
    } else {
        // 向外抛错
        MMLTaskCompletionBlock block = task.taskBlock;
        if (block) {
            NSError *aError = [NSError errorWithDomain:MMLTaskQueueErrorDomain code:MMLTaskQueueNULLMachine userInfo:nil];
            [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", aError.domain.description, aError.code, aError.userInfo]];
            block(nil, aError);
        }
        [self.logger debugLogMsg:@"execute task finish"];
        
        dispatch_async(self.serial_queue, ^{
            [self mml_taskFinished:task];
        });
    }
}


- (void)mml_executeTaskWithMachine:(MMLBaseMachine *)machine task:(MMLTask *)task {
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
    __weak typeof (self) weakSelf = self;
    
    if (self.openPerformanceProfiler) {
        [task runPerformanceTaskWithCompletionBlock:^(NSError * _Nullable error) {
            dispatch_async(self.serial_queue, ^{
                NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
                __strong typeof (self) strongSelf = weakSelf;
                if (error) {
                    [strongSelf.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", error.domain.description, error.code, error.userInfo]];
                }
                [strongSelf.logger debugLogMsg:@"execute task finish"];
                [strongSelf.logger performanceInfoLogMsg:[NSString stringWithFormat:@"execute task cost time : %.3f", (end - start) * 1000]];
                [strongSelf mml_taskFinished:task];
            });
        }];
    } else {
        [task runTaskWithCompletionBlock:^(NSError * _Nullable error) {
            dispatch_async(self.serial_queue, ^{
                NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
               __strong typeof (self) strongSelf = weakSelf;
                if (error) {
                    [strongSelf.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", error.domain.description, error.code, error.userInfo]];
                }
                [strongSelf.logger debugLogMsg:@"execute task finish"];
                [strongSelf.logger performanceInfoLogMsg:[NSString stringWithFormat:@"execute task cost time : %.3f", (end - start) * 1000]];
                [strongSelf mml_taskFinished:task];
            });
        }];
    }
}

/// 任务出队
/// @param task 待出队的任务
- (void)mml_taskFinished:(MMLTask *)task {
    [self.logger debugLogMsg:@"dequeue the finish task start"];
    if (task && [task isKindOfClass:[MMLTask class]]) {
        [self.taskStack removeObject:task];
    }
    [self.logger debugLogMsg:[NSString stringWithFormat:@"dequeue the finish task end, 队列剩余任务数目 == %lu", (unsigned long)self.taskStack.count]];
    self.queueStatus = MMLTaskQueueStatusFree;
    
    [self mml_removeMachineByTask:task];
    [self mml_performTaskQueue];
}

#pragma mark - Private

- (void)mml_releaseMachineInQueueMachines {
    for (NSString *machineId in self.queueMachines.allKeys) {
        MMLBaseMachine *machine = [self.queueMachines objectForKey:machineId];
        [machine releaseMachine];
    }
}

- (void)mml_clearMachineInQueueMachines {
    for (NSString *machineId in self.queueMachines.allKeys) {
        MMLBaseMachine *machine = [self.queueMachines objectForKey:machineId];
        [machine clearMachine];
    }
}

- (void)mml_addMachineByTask:(MMLTask *)task {
    if (![self.queueMachines objectForKey:task.machine.machineId]) {
        // store service
        if (task.machine && task.machine.machineId) {
            [self.queueMachines setObject:task.machine forKey:task.machine.machineId];
        }
    }
}

- (void)mml_removeMachineByTask:(MMLTask *)task {
    BOOL needRemoveMachineService = YES;
    for (MMLTask *taskInStack in self.taskStack) {
        if ([task.machine.machineId isEqualToString:taskInStack.machine.machineId]) {
            needRemoveMachineService = NO;
            break;
        }
    }

    if (needRemoveMachineService && [self.queueMachines objectForKey:task.machine.machineId]) {
        [self.queueMachines removeObjectForKey:task.machine.machineId];
    }
}

- (void)mml_internalAddTask:(MMLTask *)task atHead:(BOOL)atHead error:(NSError **)aError {
    if (task && [task isKindOfClass:[MMLTask class]]) {
        dispatch_async(self.serial_queue, ^{
            if (atHead) { // 插到队尾
                [self.taskStack insertObject:task atIndex:0];
            } else { // 插到队首
                [self.taskStack addObject:task];
            }
            
            [self mml_addMachineByTask:task];
            [self mml_performTaskQueue];
        });
    } else {
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLTaskQueueErrorDomain code:MMLTaskQueueParamError userInfo:nil];
            [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
        }
    }
}

- (void)mml_internalBatchAddTasks:(NSArray <MMLTask *> *)tasks atHead:(BOOL)atHead error:(NSError **)aError {
    if (tasks && [tasks isKindOfClass:[NSArray class]] && tasks.count > 0) {
        dispatch_async(self.serial_queue, ^{
            if (atHead) {
                [self.taskStack insertObjects:tasks atIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, tasks.count)]];
            } else {
                [self.taskStack addObjectsFromArray:tasks];
            }
            
            for (NSInteger index = 0; index <= tasks.count - 1; index++) {
                [self mml_addMachineByTask:tasks[index]];
            }
            [self mml_performTaskQueue];
        });
    } else {
        if (aError != NULL) {
            *aError = [NSError errorWithDomain:MMLTaskQueueErrorDomain code:MMLTaskQueueParamError userInfo:nil];
            [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg -- domain:%@, code:%ld, ext:%@", (*aError).domain.description, (*aError).code, (*aError).userInfo]];
        }
    }
}


#pragma mark - Getter

- (NSMutableArray <MMLTask *> *)taskStack {
    if (!_taskStack) {
        _taskStack = [[NSMutableArray alloc] init];
    }
    return _taskStack;
}

- (NSMutableDictionary <NSString *, MMLBaseMachine *> *)queueMachines {
    if (!_queueMachines) {
        _queueMachines = [NSMutableDictionary dictionaryWithCapacity:1];
    }
    return _queueMachines;
}

@end
