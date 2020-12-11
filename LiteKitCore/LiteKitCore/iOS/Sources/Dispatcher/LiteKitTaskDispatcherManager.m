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


#import "LiteKitTaskDispatcherManager.h"


#define LOCK(lock) dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
#define UNLOCK(lock) dispatch_semaphore_signal(lock);

@interface LiteKitTaskDispatcherManager () 

@property (nonatomic, strong) NSMutableDictionary <NSString *, LiteKitTaskQueue *> *queueMap;

@property (nonatomic, strong) dispatch_semaphore_t lock;

@property (nonatomic, strong, readwrite) NSArray <NSString *> *businessIds;
@end

@implementation LiteKitTaskDispatcherManager

#pragma mark - Init

+ (instancetype)sharedInstance {
    static LiteKitTaskDispatcherManager *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[LiteKitTaskDispatcherManager alloc] init];
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
/// apply a TaskQueue
/// @param businessId task ID
- (LiteKitTaskQueue *)applyLiteKitTaskQueueWithBusinessId:(NSString *)businessId {
    if (!businessId || ![businessId isKindOfClass:[NSString class]]) {
        return nil;
    }
    LiteKitTaskQueue *taskQueue = [self litekit_queueForKey:businessId];
    if (taskQueue) { // task id existing a Queue
        return taskQueue;
    } else {
        // create a queue
        taskQueue = [[LiteKitTaskQueue alloc] init];
        [self litekit_addQueue:taskQueue forKey:businessId];
        return taskQueue;
    }
}

- (void)removeLiteKitTaskQueueWithBusinessId:(NSString *)businessId {
    LiteKitTaskQueue *queue = [self litekit_queueForKey:businessId];
    [queue releaseMachine];
    [self litekit_deleteQueueForKey:businessId];
}

#pragma mark - Private

- (LiteKitTaskQueue *)litekit_queueForKey:(NSString *)key {
    LOCK(self.lock);
    LiteKitTaskQueue *queue = self.queueMap[key];
    UNLOCK(self.lock);
    return queue;
}

- (void)litekit_addQueue:(LiteKitTaskQueue *)queue forKey:(NSString *)key {
    LOCK(self.lock);
    if (key && [key isKindOfClass:[NSString class]]) {
        self.queueMap[key] = queue;
    }
    UNLOCK(self.lock);
}

- (void)litekit_deleteQueueForKey:(NSString *)key {
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
