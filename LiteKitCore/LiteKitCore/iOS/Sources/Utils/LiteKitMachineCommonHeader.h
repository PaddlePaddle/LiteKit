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


/// Machine的公共头文件
#ifndef LiteKitMachineCommonHeader_h
#define LiteKitMachineCommonHeader_h

#import "LiteKitCommonDefines.h"

/// error domain
#define LiteKitPaddleMachineInitErrorDomain       @"PaddleMachineInitError"
#define LiteKitPaddleMachinePredicateErrorDomain  @"PaddleMachinePredicateError"
#define LiteKitBMLMachinePredicateErrorDomain     @"BMLMachinePredicateError"

/// error key
/// predict阶段错误中userinfo字段中错误信息的key值
#define LiteKitMachinePredictErrorExtKey          @" litekit_error_ext_key"

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

/// error code
typedef NS_ENUM(NSInteger, LiteKitMachineInitErrorCode) {
    LiteKitMachineInitFailed = 0,       // machine初始化失败
    LiteKitMachineLoadFailed = 1,       // machine load阶段失败
    LiteKitModelFileNotFound = 2,       // 未找到模型文件
    LiteKitIllegalModelFileURL = 3,     // 非法的模型地址
    LiteKitIllegalOSVersion = 4,        // 非法系统版本
    LiteKitIllegalModelConfig = 5       // 错误的配置
};

/// error code
typedef NS_ENUM(NSInteger, LiteKitMachinePredicateErrorCode) {
    LiteKitMachinePredicateDestroyed = 0,                   // machine已经销毁
    LiteKitMachinePredicateInputPixelTooLarge = 1,          // input的size过大
    LiteKitMachinePredicatePredictError = 2,                // 预测阶段错误
    LiteKitMachinePredicateLoadError = 3,                   // 加载错误
    LiteKitMachinePredicateUpdateDimError = 4,              // 更新dim失败
    LiteKitMachinePredicateConvertTextureError = 5,         // 转换texture失败
    LiteKitMachinePredicateInputDataError = 6,              // 输入数据有误
    LiteKitMachinePredicateNotSupportSimulator = 7,         // 不支持模拟器
    LiteKitMachinePredicateNotSupportArchitecture = 8,      // 不支持的处理器架构
    LiteKitMachinePredicateMachineNotReady = 9,             // Machine未准备好
    LiteKitMachinePredicateMachineReloadError = 10          // 模型reload失败
};

#endif /* LiteKitMachineCommonHeader_h */
