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
