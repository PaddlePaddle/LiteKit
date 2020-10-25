//
//  MMLLogger.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/26.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLLogger.h"


#ifdef DEBUG
static NSUInteger tureConsoleLogLevel = MMLlogLevelTypeForDebug;       // 调试期的log级别
#else
static NSUInteger tureConsoleLogLevel = -1;                            // 非调试期的log级别
#endif


@interface MMLLogger ()

@property (nonatomic, strong) NSString *mmlTag;

- (void)consolelogMsg:(NSString *)content level:(NSInteger)level;

@end


@implementation MMLLogger

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
                case MMLlogLevelTypeForError:
                    contentLevel = @"❌";
                    break;
                case MMLlogLevelTypeForPerformanceInfo:
                    contentLevel = @"📶";
                    break;
                case MMLlogLevelTypeForWarning:
                    contentLevel = @"⚠️";
                    break;
                case MMLlogLevelTypeForDebug:
                    contentLevel = @"⚙️";
                    break;
                default:
                    break;
            }
            NSLog((@"【MMLLog】【level : %@】 【Tag : %@】 %@"), contentLevel, self.mmlTag?: [NSString stringWithUTF8String:__FILE__], content);
    //        NSLog(@"mml_log_info: current phase: %@, extra info: %@",phaseName,dictionary);
        }
}

#pragma mark - MMLLoggerProtocol

- (void)setLogTag:(NSString *)tag {
    self.mmlTag = tag;
}

- (void)debugLogMsg:(NSString *)content {
    [self consolelogMsg:content level:MMLlogLevelTypeForDebug];
}

- (void)errorLogMsg:(NSString *)content {
    [self consolelogMsg:content level:MMLlogLevelTypeForError];
}


- (void)performanceInfoLogMsg:(NSString *)content {
    [self consolelogMsg:content level:MMLlogLevelTypeForPerformanceInfo];
}


- (void)warningLogMsg:(NSString *)content {
    [self consolelogMsg:content level:MMLlogLevelTypeForWarning];
}

@end


void MMLlogMessageFunc(NSInteger level, const char *file, int lineNumber, const char *functionName, NSString *format, ...) {
    va_list args;
    if (format) {
        va_start(args, format);
        NSString *message = [[NSString alloc] initWithFormat:format arguments:args];
        [[MMLLogger new] consolelogMsg:message level:level];
        va_end(args);
    }
}
