//
//  MMLTaskDispatcherManager.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/21.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLTaskQueue.h"
#import "MMLTask.h"

NS_ASSUME_NONNULL_BEGIN

/// 任务分发管理器
@interface MMLTaskDispatcherManager : NSObject
/// 所有的业务ID
@property (nonatomic, strong, readonly) NSArray <NSString *> *businessIds;

/// 单实例
+ (instancetype)sharedInstance;

/// 申请TaskQueue
/// @param businessId 业务ID
- (MMLTaskQueue *)applyMMLTaskQueueWithBusinessId:(NSString * __nullable)businessId;


/// 移除TaskQueue
/// @param businessId 业务ID
- (void)removeMMLTaskQueueWithBusinessId:(NSString * __nullable)businessId;


@end

NS_ASSUME_NONNULL_END
