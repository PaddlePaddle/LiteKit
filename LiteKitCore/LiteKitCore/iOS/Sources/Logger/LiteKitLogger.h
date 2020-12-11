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
    LiteKitlogLevelTypeForError = 0,        // error level，use when error occur
    LiteKitlogLevelTypeForPerformanceInfo,  // performance level，for output load、predict speed
    LiteKitlogLevelTypeForWarning,          // warning level，using in dealloc period
    LiteKitlogLevelTypeForDebug             // debug level，common debug log
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

/// LiteKit default logger
@interface LiteKitLogger : NSObject <LiteKitLoggerProtocol>

/**
 set console log output level，DEBUG default output all，else output error level
 */
@property (nonatomic, assign) NSUInteger consoleLogLevel;

/// init logger with tag
/// @param tag tag
- (instancetype)initWithTag:(NSString *)tag;

@end

NS_ASSUME_NONNULL_END
