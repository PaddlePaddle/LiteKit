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
