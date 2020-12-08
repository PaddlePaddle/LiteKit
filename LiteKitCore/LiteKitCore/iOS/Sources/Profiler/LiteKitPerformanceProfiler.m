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


#import "LiteKitPerformanceProfiler.h"

NSString * const LiteKitPerformanceLoadTimeForInferenceEngine    = @"LiteKitPerformanceLoadTimeForInferenceEngine";        // 纯load时间
NSString * const LiteKitPerformancePredictTimeForInferenceEngine = @"LiteKitPerformancePredictTimeForInferenceEngine";     // 纯预测时间
NSString * const LiteKitPerformanceReadMetalSourceTime           = @"LiteKitPerformanceReadMetalSourceTime";               // 读取metallib资源时间
NSString * const LiteKitPerformanceLoadTimeForInterface          = @"LiteKitPerformanceLoadTimeForInterface";              // 接口层级的load时间
NSString * const LiteKitPerformancePredictTimeForInterface       = @"LiteKitPerformancePredictTimeForInterface";           // 接口层级的预测时间

@interface LiteKitPerformanceProfiler ()
@property (atomic, strong, readwrite) NSDictionary *performanceMap;
@end

@implementation LiteKitPerformanceProfiler

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
