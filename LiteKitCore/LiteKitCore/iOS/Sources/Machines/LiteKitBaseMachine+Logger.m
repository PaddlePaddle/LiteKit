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


#import "LiteKitBaseMachine+Logger.h"
#import <objc/runtime.h>

@implementation LiteKitMachine (Logger)

- (void)setLogger:(id<LiteKitLoggerProtocol>)logger {
    objc_setAssociatedObject(self, @selector(logger), logger, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (id <LiteKitLoggerProtocol>)logger {
    return objc_getAssociatedObject(self, _cmd);
}

- (void)setupMachineLoggerFromMachineLoggerName:(NSString *)loggerClassName {
    if ([loggerClassName isKindOfClass:[NSString class]] &&
        loggerClassName.length > 0 &&
        NSClassFromString(loggerClassName) &&
        [NSClassFromString(loggerClassName) instancesRespondToSelector:@selector(setLogTag:)]) {
        id <LiteKitLoggerProtocol> logger = [NSClassFromString(loggerClassName) new];
        [logger setLogTag:LiteKitMachineLoggerTag];
        [self setLogger:logger];
    } else {
#if __has_include("LiteKitLogger.h")
    id <LiteKitLoggerProtocol> logger = [[LiteKitLogger alloc] initWithTag:LiteKitMachineLoggerTag];
    [self setLogger:logger];
#endif
    }
}


@end
