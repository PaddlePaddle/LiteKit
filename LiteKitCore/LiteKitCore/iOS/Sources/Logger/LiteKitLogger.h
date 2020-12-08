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
#import "LiteKitLoggerProtocol.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LiteKitlogLevelType) {
    LiteKitlogLevelTypeForError = 0,        // 错误级别，一般用于报错时期
    LiteKitlogLevelTypeForPerformanceInfo,  // 性能信息级别，用于加载、预测速度打印等
    LiteKitlogLevelTypeForWarning,          // 警告级别，一般用于dealloc时期
    LiteKitlogLevelTypeForDebug             // 调试信息级别，用于多数场景，流程转换时期等
};


FOUNDATION_EXPORT void LiteKitlogMessageFunc(NSInteger level,
                                         const char *file,
                                         int lineNumber,
                                         const char *functionName,
                                         NSString *format, ...) NS_FORMAT_FUNCTION(5,6);

#define LogVerbose(frmt, ...) \
do { LiteKitlogMessageFunc(LiteKitlogLevelTypeForDebug, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogError(frmt, ...) \
do { LiteKitlogMessageFunc(LiteKitlogLevelTypeForError, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogWarning(level, frmt, ...) \
do { LiteKitlogMessageFunc(LiteKitlogLevelTypeForWarning, __FILE__, __LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)
#define LogInfo(level, frmt, ...) \
do { LiteKitlogMessageFunc(LiteKitlogLevelTypeForPerformanceInfo, __FILE__ ,__LINE__, __FUNCTION__, frmt, ##__VA_ARGS__);} while (0)

/// LiteKit 默认的logger
@interface LiteKitLogger : NSObject <LiteKitLoggerProtocol>

/**
 设置控制台log输出的等级，DEBUG默认输出所有等级，其他默认输出error等级
 */
@property (nonatomic, assign) NSUInteger consoleLogLevel;

/// 根据tag标签进行初始化logger
/// @param tag tag标签
- (instancetype)initWithTag:(NSString *)tag;

@end

NS_ASSUME_NONNULL_END
