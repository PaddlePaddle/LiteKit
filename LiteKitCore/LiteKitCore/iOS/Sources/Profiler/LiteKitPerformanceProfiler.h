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
