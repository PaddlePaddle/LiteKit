//
//  MMLPerformanceProfiler.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/6.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString * const MMLPerformanceLoadTimeForInferenceEngine;        // 纯load时间
FOUNDATION_EXPORT NSString * const MMLPerformancePredictTimeForInferenceEngine;     // 纯预测时间
FOUNDATION_EXPORT NSString * const MMLPerformanceReadMetalSourceTime;               // 读取metallib资源时间
FOUNDATION_EXPORT NSString * const MMLPerformanceLoadTimeForInterface;              // 接口层级的load时间
FOUNDATION_EXPORT NSString * const MMLPerformancePredictTimeForInterface;           // 接口层级的预测时间


/// 性能统计实体类
@interface MMLPerformanceProfiler : NSObject

/// 性能数据Map
@property (atomic, strong, readonly) NSDictionary *performanceMap;

/// 添加性能数据
/// @param data 性能数据
/// @param key 性能数据对应的Key
- (void)appendPerformanceData:(NSString *)data key:(NSString *)key;

@end

NS_ASSUME_NONNULL_END
