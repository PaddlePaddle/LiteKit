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
#import "LiteKitData.h"
#import "LiteKitMachine.h"
#import "LiteKitMachineCommonHeader.h"
#import "LiteKitPerformanceProfiler.h"

// error domain
FOUNDATION_EXPORT NSString * _Nonnull const LiteKitMachinInitErrorDomain;//模型初始化失败 Domain
FOUNDATION_EXPORT NSString * _Nonnull const LiteKitMachinPredicateErrorDomain;//预测Error Domain

NS_ASSUME_NONNULL_BEGIN

/// @param outputData output数据
/// @param error error信息
typedef void (^LiteKitMachinePredictCompletionBlock)(LiteKitData * __nullable outputData, NSError * __nullable error);


/// @param outputData output数据
/// @param performanceProfiler performance信息
/// @param error error信息
typedef void (^LiteKitMachinePredictPerformanceBlock)(LiteKitData * __nullable outputData,
                                                  LiteKitPerformanceProfiler * __nullable performanceProfiler,
                                                  NSError * __nullable error);

/// Machine的基类
@interface LiteKitBaseMachine : LiteKitMachine

/// machine唯一ID
@property (nonatomic, copy, readonly) NSString * machineId;

#pragma mrak - Predict
/// sync
/// @param aInputData LiteKitData类型的输入数据
/// @param aError 错误信息
- (LiteKitData * __nullable)predictWithInputData:(LiteKitData * __nonnull)aInputData error:(NSError **)aError;

/// async
/// @param aInputData LiteKitData类型的输入数据
/// @param aBlock 完成回调
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock;

/// 运行带performance信息回调的machine
/// @param aInputData input数据
/// @param aBlock 带performance信息的回调
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData performanceBlock:(LiteKitMachinePredictPerformanceBlock)aBlock;

#pragma mark - Clear

/// 清理machine内存，不释放Machine
- (void)clearMachine;

#pragma mark - Release

/// 释放Machine,置空Machine
- (void)releaseMachine;

@end

NS_ASSUME_NONNULL_END
