//
//  MMLBaseMachine.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/16.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
