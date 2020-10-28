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
