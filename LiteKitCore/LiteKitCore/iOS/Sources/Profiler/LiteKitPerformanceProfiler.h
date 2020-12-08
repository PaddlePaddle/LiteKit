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

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString * const LiteKitPerformanceLoadTimeForInferenceEngine;        // 纯load时间
FOUNDATION_EXPORT NSString * const LiteKitPerformancePredictTimeForInferenceEngine;     // 纯预测时间
FOUNDATION_EXPORT NSString * const LiteKitPerformanceReadMetalSourceTime;               // 读取metallib资源时间
FOUNDATION_EXPORT NSString * const LiteKitPerformanceLoadTimeForInterface;              // 接口层级的load时间
FOUNDATION_EXPORT NSString * const LiteKitPerformancePredictTimeForInterface;           // 接口层级的预测时间


/// 性能统计实体类
@interface LiteKitPerformanceProfiler : NSObject

/// 性能数据Map
@property (atomic, strong, readonly) NSDictionary *performanceMap;

/// 添加性能数据
/// @param data 性能数据
/// @param key 性能数据对应的Key
- (void)appendPerformanceData:(NSString *)data key:(NSString *)key;

@end

NS_ASSUME_NONNULL_END
