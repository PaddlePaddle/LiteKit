//
//  MMLMachineCommonHeader.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/14.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
