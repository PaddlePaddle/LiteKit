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
#import "LiteKitMachineService.h"

NS_ASSUME_NONNULL_BEGIN
/// @param machine 加载成功的machine
/// @param performanceProfiler performance信息
/// @param error error信息
typedef void (^LiteKitMachineLoadPerformanceBlock)(LiteKitMachine * __nullable machine,
                                               LiteKitPerformanceProfiler * __nullable performanceProfiler,
                                               NSError * __nullable error);

/// 带performance信息的Service分类
@interface LiteKitMachineService (Performancer)

/// 加载带performance信息回调的machine
/// @param aConfig 配置
/// @param aBlock 带performance信息的回调
- (void)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig performanceBlock:(LiteKitMachineLoadPerformanceBlock)aBlock;
@end

NS_ASSUME_NONNULL_END