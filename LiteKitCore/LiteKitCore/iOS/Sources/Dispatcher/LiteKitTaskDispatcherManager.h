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
#import "LiteKitTaskQueue.h"
#import "LiteKitTask.h"

NS_ASSUME_NONNULL_BEGIN

/// 任务分发管理器
@interface LiteKitTaskDispatcherManager : NSObject
/// 所有的业务ID
@property (nonatomic, strong, readonly) NSArray <NSString *> *businessIds;

/// 单实例
+ (instancetype)sharedInstance;

/// 申请TaskQueue
/// @param businessId 业务ID
- (LiteKitTaskQueue *)applyLiteKitTaskQueueWithBusinessId:(NSString * __nullable)businessId;


/// 移除TaskQueue
/// @param businessId 业务ID
- (void)removeLiteKitTaskQueueWithBusinessId:(NSString * __nullable)businessId;


@end

NS_ASSUME_NONNULL_END
