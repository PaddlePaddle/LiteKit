//
//  MMLLoggerProtocol.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/28.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
