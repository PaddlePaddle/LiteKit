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
