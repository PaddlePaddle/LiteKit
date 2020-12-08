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
#import "LiteKitBaseMachine.h"
#import "LiteKitMachineConfig.h"
#import "LiteKitData.h"
#import "LiteKitPerformanceProfiler.h"

NS_ASSUME_NONNULL_BEGIN
/// load阶段错误domain
FOUNDATION_EXPORT NSString * const LiteKitMachineServiceLoadErrorDomain;
/// error信息中的附带信息key值
FOUNDATION_EXPORT NSString * const LiteKitMachineServiceErrorExtKey;

/// service load阶段错误值枚举
typedef NS_ENUM(NSInteger,LiteKitMachineServiceLoadErrorCode) {
    LiteKitMachineServiceLoadMachineTypeError = 0,      // 错误的machine类型
    LiteKitMachineServiceLoadNotSupportSimulator,       // 不支持模拟器
    LiteKitMachineServiceLoadNotSupportArchitecture,    // 不支持的处理器架构
    LiteKitMachineServiceLoadWrongConfig,               // 错误的配置
    LiteKitMachineServiceLoadNoModelFile,               // 没有模型文件
    LiteKitMachineServiceLoadNoModelPointer,            // 内存指针为空
};


/// @param machine 加载成功的machine
/// @param error error信息
typedef void (^LiteKitMachineLoadCompletionBlock)(LiteKitMachine * __nullable machine, NSError * __nullable error);


/// Machine Service类，负责Machine的创建、运行以及释放
@interface LiteKitMachineService : NSObject

/// 当前对应的machine
@property (nonatomic, strong, readonly) LiteKitBaseMachine * __nullable litekitMachine;
/// Service的唯一标识
@property (nonatomic, copy, readonly) NSString *machineServiceId;
/// 性能统计数据
@property (nonatomic, strong, readonly) LiteKitPerformanceProfiler *performanceDataMap;

#pragma mark - Load

/// sync 通过配置加载Machine
/// @param aConfig LiteKit配置
/// @param aError 错误信息
- (LiteKitBaseMachine * __nullable)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig error:(NSError **)aError;

/// async
/// @param aConfig LiteKit配置
/// @param aBlock 完成回调
- (void)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig completionBlock:(LiteKitMachineLoadCompletionBlock)aBlock;


@end

NS_ASSUME_NONNULL_END
