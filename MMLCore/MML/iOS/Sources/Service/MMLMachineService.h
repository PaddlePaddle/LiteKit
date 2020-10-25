//
//  MMLMachineService.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/14.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLBaseMachine.h"
#import "MMLMachineConfig.h"
#import "MMLData.h"
#import "MMLPerformanceProfiler.h"

NS_ASSUME_NONNULL_BEGIN
/// load阶段错误domain
FOUNDATION_EXPORT NSString * const MMLMachineServiceLoadErrorDomain;
/// error信息中的附带信息key值
FOUNDATION_EXPORT NSString * const MMLMachineServiceErrorExtKey;

/// service load阶段错误值枚举
typedef NS_ENUM(NSInteger,MMLMachineServiceLoadErrorCode) {
    MMLMachineServiceLoadMachineTypeError = 0,      // 错误的machine类型
    MMLMachineServiceLoadNotSupportSimulator,       // 不支持模拟器
    MMLMachineServiceLoadNotSupportArchitecture,    // 不支持的处理器架构
    MMLMachineServiceLoadWrongConfig,               // 错误的配置
    MMLMachineServiceLoadNoModelFile,               // 没有模型文件
    MMLMachineServiceLoadNoModelPointer,            // 内存指针为空
};


/// @param machine 加载成功的machine
/// @param error error信息
typedef void (^MMLMachineLoadCompletionBlock)(MMLMachine * __nullable machine, NSError * __nullable error);


/// Machine Service类，负责Machine的创建、运行以及释放
@interface MMLMachineService : NSObject

/// 当前对应的machine
@property (nonatomic, strong, readonly) MMLBaseMachine * __nullable mmlMachine;
/// Service的唯一标识
@property (nonatomic, copy, readonly) NSString *machineServiceId;
/// 性能统计数据
@property (nonatomic, strong, readonly) MMLPerformanceProfiler *performanceDataMap;

#pragma mark - Load

/// sync 通过配置加载Machine
/// @param aConfig MML配置
/// @param aError 错误信息
- (MMLBaseMachine * __nullable)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig error:(NSError **)aError;

/// async
/// @param aConfig MML配置
/// @param aBlock 完成回调
- (void)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig completionBlock:(MMLMachineLoadCompletionBlock)aBlock;


@end

NS_ASSUME_NONNULL_END
