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


#import "LiteKitLogger.h"


#ifdef DEBUG
static NSUInteger tureConsoleLogLevel = LiteKitlogLevelTypeForDebug;       // 调试期的log级别
#else
static NSUInteger tureConsoleLogLevel = -1;                            // 非调试期的log级别
#endif


@interface LiteKitLogger ()

@property (nonatomic, strong) NSString *litekitTag;

- (void)consolelogMsg:(NSString *)content level:(NSInteger)level;

@end


@implementation LiteKitLogger

- (instancetype)initWithTag:(NSString *)tag {
    self = [super init];
    if (self) {
        [self setLogTag:tag];
    }
    return self;
}

- (void)consolelogMsg:(NSString *)content level:(NSInteger)level {
    if (level <= tureConsoleLogLevel) {
            NSString *contentLevel = nil;
            switch (level) {
                case LiteKitlogLevelTypeForError:
                    contentLevel = @"❌";
                    break;
                case LiteKitlogLevelTypeForPerformanceInfo:
                    contentLevel = @"📶";
                    break;
                case LiteKitlogLevelTypeForWarning:
                    contentLevel = @"⚠️";
                    break;
                case LiteKitlogLevelTypeForDebug:
                    contentLevel = @"⚙️";
                    break;
                default:
                    break;
            }
            NSLog((@"【LiteKitLog】【level : %@】 【Tag : %@】 %@"), contentLevel, self.litekitTag?: [NSString stringWithUTF8String:__FILE__], content);
    //        NSLog(@"litekit_log_info: current phase: %@, extra info: %@",phaseName,dictionary);
        }
}

#pragma mark - LiteKitLoggerProtocol

- (void)setLogTag:(NSString *)tag {
    self.litekitTag = tag;
}

- (void)debugLogMsg:(NSString *)content {
    [self consolelogMsg:content level:LiteKitlogLevelTypeForDebug];
}

- (void)errorLogMsg:(NSString *)content {
    [self consolelogMsg:content level:LiteKitlogLevelTypeForError];
}


- (void)performanceInfoLogMsg:(NSString *)content {
    [self consolelogMsg:content level:LiteKitlogLevelTypeForPerformanceInfo];
}


- (void)warningLogMsg:(NSString *)content {
    [self consolelogMsg:content level:LiteKitlogLevelTypeForWarning];
}

@end


void LiteKitlogMessageFunc(NSInteger level, const char *file, int lineNumber, const char *functionName, NSString *format, ...) {
    va_list args;
    if (format) {
        va_start(args, format);
        NSString *message = [[NSString alloc] initWithFormat:format arguments:args];
        [[LiteKitLogger new] consolelogMsg:message level:level];
        va_end(args);
    }
}
