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
