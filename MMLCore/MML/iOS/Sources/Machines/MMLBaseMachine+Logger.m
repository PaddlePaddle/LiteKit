//
//  MMLMachine+Logger.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/29.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLBaseMachine+Logger.h"
#import <objc/runtime.h>

@implementation MMLMachine (Logger)

- (void)setLogger:(id<MMLLoggerProtocol>)logger {
    objc_setAssociatedObject(self, @selector(logger), logger, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (id <MMLLoggerProtocol>)logger {
    return objc_getAssociatedObject(self, _cmd);
}

- (void)setupMachineLoggerFromMachineLoggerName:(NSString *)loggerClassName {
    if ([loggerClassName isKindOfClass:[NSString class]] &&
        loggerClassName.length > 0 &&
        NSClassFromString(loggerClassName) &&
        [NSClassFromString(loggerClassName) instancesRespondToSelector:@selector(setLogTag:)]) {
        id <MMLLoggerProtocol> logger = [NSClassFromString(loggerClassName) new];
        [logger setLogTag:MMLMachineLoggerTag];
        [self setLogger:logger];
    } else {
#if __has_include("MMLLogger.h")
    id <MMLLoggerProtocol> logger = [[MMLLogger alloc] initWithTag:MMLMachineLoggerTag];
    [self setLogger:logger];
#endif
    }
}


@end
