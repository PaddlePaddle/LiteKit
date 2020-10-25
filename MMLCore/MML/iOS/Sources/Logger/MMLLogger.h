//
//  MMLLogger.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/26.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
