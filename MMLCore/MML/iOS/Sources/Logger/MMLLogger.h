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
#import "MMLLoggerProtocol.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, MMLlogLevelType) {
    MMLlogLevelTypeForError = 0,        // 错误级别，一般用于报错时期
    MMLlogLevelTypeForPerformanceInfo,  // 性能信息级别，用于加载、预测速度打印等
    MMLlogLevelTypeForWarning,          // 警告级别，一般用于dealloc时期
    MMLlogLevelTypeForDebug             // 调试信息级别，用于多数场景，流程转换时期等
};


FOUNDATION_EXPORT void MMLlogMessageFunc(NSInteger level,
                                         const char *file,
                                         int lineNumber,
                                         const char *functionName,
                                         NSString *format, ...) NS_FORMAT_FUNCTION(5,6);

#define LogVerbose(frmt, ...) \
do { MMLlogMessageFunc(MMLlogLevelTypeForDebug, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogError(frmt, ...) \
do { MMLlogMessageFunc(MMLlogLevelTypeForError, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogWarning(level, frmt, ...) \
do { MMLlogMessageFunc(MMLlogLevelTypeForWarning, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogInfo(level, frmt, ...) \
do { MMLlogMessageFunc(MMLlogLevelTypeForPerformanceInfo, __FILE__ ,__LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)

/// MML 默认的logger
@interface MMLLogger : NSObject <MMLLoggerProtocol>

/**
 设置控制台log输出的等级，DEBUG默认输出所有等级，其他默认输出error等级
 */
@property (nonatomic, assign) NSUInteger consoleLogLevel;

/// 根据tag标签进行初始化logger
/// @param tag tag标签
- (instancetype)initWithTag:(NSString *)tag;

@end

NS_ASSUME_NONNULL_END
