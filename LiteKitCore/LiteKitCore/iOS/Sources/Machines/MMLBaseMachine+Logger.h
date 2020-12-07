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
