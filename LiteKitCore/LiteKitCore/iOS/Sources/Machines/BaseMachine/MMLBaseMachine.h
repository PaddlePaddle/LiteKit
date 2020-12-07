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
#import "MMLData.h"
#import "MMLMachine.h"
#import "MMLMachineCommonHeader.h"
#import "MMLPerformanceProfiler.h"

// error domain
FOUNDATION_EXPORT NSString * _Nonnull const MMLMachinInitErrorDomain;//模型初始化失败 Domain
FOUNDATION_EXPORT NSString * _Nonnull const MMLMachinPredicateErrorDomain;//预测Error Domain

NS_ASSUME_NONNULL_BEGIN

/// @param outputData output数据
/// @param error error信息
typedef void (^MMLMachinePredictCompletionBlock)(MMLData * __nullable outputData, NSError * __nullable error);


/// @param outputData output数据
/// @param performanceProfiler performance信息
/// @param error error信息
typedef void (^MMLMachinePredictPerformanceBlock)(MMLData * __nullable outputData,
                                                  MMLPerformanceProfiler * __nullable performanceProfiler,
                                                  NSError * __nullable error);

/// Machine的基类
@interface MMLBaseMachine : MMLMachine

/// machine唯一ID
@property (nonatomic, copy, readonly) NSString * machineId;

#pragma mrak - Predict
/// sync
/// @param aInputData MMLData类型的输入数据
/// @param aError 错误信息
- (MMLData * __nullable)predictWithInputData:(MMLData * __nonnull)aInputData error:(NSError **)aError;

/// async
/// @param aInputData MMLData类型的输入数据
/// @param aBlock 完成回调
- (void)predictWithInputData:(MMLData * __nonnull)aInputData completionBlock:(MMLMachinePredictCompletionBlock)aBlock;

/// 运行带performance信息回调的machine
/// @param aInputData input数据
/// @param aBlock 带performance信息的回调
- (void)predictWithInputData:(MMLData * __nonnull)aInputData performanceBlock:(MMLMachinePredictPerformanceBlock)aBlock;

#pragma mark - Clear

/// 清理machine内存，不释放Machine
- (void)clearMachine;

#pragma mark - Release

/// 释放Machine,置空Machine
- (void)releaseMachine;

@end

NS_ASSUME_NONNULL_END
