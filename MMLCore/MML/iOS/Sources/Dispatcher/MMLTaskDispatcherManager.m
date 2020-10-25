//
//  MMLTaskDispatcherManager.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/21.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLTaskDispatcherManager.h"


#define LOCK(lock) dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
#define UNLOCK(lock) dispatch_semaphore_signal(lock);

@interface MMLTaskDispatcherManager () 

@property (nonatomic, strong) NSMutableDictionary <NSString *, MMLTaskQueue *> *queueMap;

@property (nonatomic, strong) dispatch_semaphore_t lock;

@property (nonatomic, strong, readwrite) NSArray <NSString *> *businessIds;
@end

@implementation MMLTaskDispatcherManager

#pragma mark - Init

+ (instancetype)sharedInstance {
    static MMLTaskDispatcherManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[MMLTaskDispatcherManager alloc] init];
    });
    
    return instance;
}

- (instancetype)init {
    if (self = [super init]) {
        _lock = dispatch_semaphore_create(1);
        _queueMap = [NSMutableDictionary dictionary];
    }
    
    return self;
}

#pragma mark - Public
/// 申请一个TaskQueue
/// @param businessId 业务ID
- (MMLTaskQueue *)applyMMLTaskQueueWithBusinessId:(NSString *)businessId {
    if (!businessId || ![businessId isKindOfClass:[NSString class]]) {
        return nil;
    }
    MMLTaskQueue *taskQueue = [self mml_queueForKey:businessId];
    if (taskQueue) { // 基于该业务ID已经存在相应的绑定
        return taskQueue;
    } else {
        // 创建一个queue
        taskQueue = [[MMLTaskQueue alloc] init];
        [self mml_addQueue:taskQueue forKey:businessId];
        return taskQueue;
    }
}

- (void)removeMMLTaskQueueWithBusinessId:(NSString *)businessId {
    MMLTaskQueue *queue = [self mml_queueForKey:businessId];
    [queue releaseMachine];
    [self mml_deleteQueueForKey:businessId];
}

#pragma mark - Private

- (MMLTaskQueue *)mml_queueForKey:(NSString *)key {
    LOCK(self.lock);
    MMLTaskQueue *queue = self.queueMap[key];
    UNLOCK(self.lock);
    return queue;
}

- (void)mml_addQueue:(MMLTaskQueue *)queue forKey:(NSString *)key {
    LOCK(self.lock);
    if (key && [key isKindOfClass:[NSString class]]) {
        self.queueMap[key] = queue;
    }
    UNLOCK(self.lock);
}

- (void)mml_deleteQueueForKey:(NSString *)key {
    LOCK(self.lock);
    [self.queueMap removeObjectForKey:key];
    UNLOCK(self.lock);
}


#pragma mark - Getter

- (NSArray <NSString *> *)businessIds {
    if (!_businessIds) {
        _businessIds = [self.queueMap allKeys];
    }
    return _businessIds;
}


@end
