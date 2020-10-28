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


#ifndef MMLLoggerProtocol_h
#define MMLLoggerProtocol_h

/// Logger 接口
@protocol MMLLoggerProtocol <NSObject>
@required
/// 设置log的Tag
/// @param tag tag标识
- (void)setLogTag:(NSString *)tag;

/// debug级别的log信息
/// @param content 内容
- (void)debugLogMsg:(NSString *)content;

/// 错误级别的log信息
/// @param content 内容
- (void)errorLogMsg:(NSString *)content;

/// 警告级别的log信息
/// @param content 内容
- (void)warningLogMsg:(NSString *)content;

/// 性能级别的log信息
/// @param content 内容
- (void)performanceInfoLogMsg:(NSString *)content;
@end

#endif /* MMLLoggerProtocol_h */
