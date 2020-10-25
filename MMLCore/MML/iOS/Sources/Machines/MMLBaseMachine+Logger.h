//
//  MMLMachine+Logger.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/29.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLBaseMachine.h"
#import "MMLMachineCommonHeader.h"

NS_ASSUME_NONNULL_BEGIN

@interface MMLMachine (Logger)

/// Machine的日志器
@property (nonatomic, strong) id <MMLLoggerProtocol> logger;

/// 设置日志器
/// @param loggerClassName 日志器类名
- (void)setupMachineLoggerFromMachineLoggerName:(NSString *)loggerClassName;

@end

NS_ASSUME_NONNULL_END
