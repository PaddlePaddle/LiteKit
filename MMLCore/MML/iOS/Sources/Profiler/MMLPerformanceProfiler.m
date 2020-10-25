//
//  MMLPerformanceProfiler.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/6.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLPerformanceProfiler.h"

NSString * const MMLPerformanceLoadTimeForInferenceEngine    = @"MMLPerformanceLoadTimeForInferenceEngine";        // 纯load时间
NSString * const MMLPerformancePredictTimeForInferenceEngine = @"MMLPerformancePredictTimeForInferenceEngine";     // 纯预测时间
NSString * const MMLPerformanceReadMetalSourceTime           = @"MMLPerformanceReadMetalSourceTime";               // 读取metallib资源时间
NSString * const MMLPerformanceLoadTimeForInterface          = @"MMLPerformanceLoadTimeForInterface";              // 接口层级的load时间
NSString * const MMLPerformancePredictTimeForInterface       = @"MMLPerformancePredictTimeForInterface";           // 接口层级的预测时间

@interface MMLPerformanceProfiler ()
@property (atomic, strong, readwrite) NSDictionary *performanceMap;
@end

@implementation MMLPerformanceProfiler

#pragma mark - Init

- (instancetype)init {
    if (self = [super init]) {
        _performanceMap = [[NSDictionary alloc] init];
    }
    return self;
}

#pragma mark - Public

- (void)appendPerformanceData:(NSString *)data key:(NSString *)key {
    if (!data ||
        !key ||
        ![data isKindOfClass:[NSString class]] ||
        ![key isKindOfClass:[NSString class]]) {
        return;
    }
    
    if ([self.performanceMap objectForKey:key]) {
        return;
    }
    
    NSMutableDictionary *performanceMapCopy = [self.performanceMap mutableCopy];
    [performanceMapCopy setObject:data forKey:key];
    self.performanceMap = [performanceMapCopy mutableCopy];
    
}

@end
