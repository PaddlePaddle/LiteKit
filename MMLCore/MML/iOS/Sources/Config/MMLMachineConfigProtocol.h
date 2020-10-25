//
//  MMLMachineConfigProtocol.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/13.
//  Copyright © 2019 Baidu. All rights reserved.
//

#ifndef MMLMachineConfigProtocol_h
#define MMLMachineConfigProtocol_h
#import "MMLInferenceEngineConfigProtocol.h"
/// Machine的类型
typedef NS_ENUM (NSUInteger, MMLMachineType) {
    MMLPaddleGPU,              // paddle GPU Machine
    MMLPaddleCPU               // paddle CPU Machine
};

/// MMLMachine的公共协议
@protocol MMLMachineConfigProtocol <NSObject>
/// Machine类型,参数必传，代表使用什么底层inference框架
@property (nonatomic, assign) MMLMachineType machineType;
/// 模型文件地址，通过已存在的Model地址加载Machine
@property (nonatomic, copy) NSString *modelPath;
/// inference引擎特殊的配置
@property (nonatomic, strong) id <MMLInferenceEngineConfigProtocol> engineConifg;
/// 可以设置自定义的logger
@property (nonatomic, copy) NSString *loggerClassName;

@end
#endif /* MMLModelConfigProtocol_h */
