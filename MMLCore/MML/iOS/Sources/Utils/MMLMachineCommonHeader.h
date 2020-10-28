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


/// Machine的公共头文件
#ifndef MMLMachineCommonHeader_h
#define MMLMachineCommonHeader_h

#import "MMLCommonDefines.h"

/// error domain
#define MMLPaddleMachineInitErrorDomain       @"PaddleMachineInitError"
#define MMLPaddleMachinePredicateErrorDomain  @"PaddleMachinePredicateError"
#define MMLBMLMachinePredicateErrorDomain     @"BMLMachinePredicateError"

/// error key
/// predict阶段错误中userinfo字段中错误信息的key值
#define MMLMachinePredictErrorExtKey          @"mml_error_ext_key"

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

/// error code
typedef NS_ENUM(NSInteger, MMLMachineInitErrorCode) {
    MMLMachineInitFailed = 0,       // machine初始化失败
    MMLMachineLoadFailed = 1,       // machine load阶段失败
    MMLModelFileNotFound = 2,       // 未找到模型文件
    MMLIllegalModelFileURL = 3,     // 非法的模型地址
    MMLIllegalOSVersion = 4,        // 非法系统版本
    MMLIllegalModelConfig = 5       // 错误的配置
};

/// error code
typedef NS_ENUM(NSInteger, MMLMachinePredicateErrorCode) {
    MMLMachinePredicateDestroyed = 0,                   // machine已经销毁
    MMLMachinePredicateInputPixelTooLarge = 1,          // input的size过大
    MMLMachinePredicatePredictError = 2,                // 预测阶段错误
    MMLMachinePredicateLoadError = 3,                   // 加载错误
    MMLMachinePredicateUpdateDimError = 4,              // 更新dim失败
    MMLMachinePredicateConvertTextureError = 5,         // 转换texture失败
    MMLMachinePredicateInputDataError = 6,              // 输入数据有误
    MMLMachinePredicateNotSupportSimulator = 7,         // 不支持模拟器
    MMLMachinePredicateNotSupportArchitecture = 8,      // 不支持的处理器架构
    MMLMachinePredicateMachineNotReady = 9,             // Machine未准备好
    MMLMachinePredicateMachineReloadError = 10          // 模型reload失败
};

#endif /* MMLMachineCommonHeader_h */
