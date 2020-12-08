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


#ifndef LiteKitMachineConfigProtocol_h
#define LiteKitMachineConfigProtocol_h
#import "LiteKitInferenceEngineConfigProtocol.h"
/// Machine的类型
typedef NS_ENUM (NSUInteger, LiteKitMachineType) {
    LiteKitPaddleGPU,              // paddle GPU Machine
    LiteKitPaddleCPU               // paddle CPU Machine
};

/// LiteKitMachine的公共协议
@protocol LiteKitMachineConfigProtocol <NSObject>
/// Machine类型,参数必传，代表使用什么底层inference框架
@property (nonatomic, assign) LiteKitMachineType machineType;
/// 模型文件地址，通过已存在的Model地址加载Machine
@property (nonatomic, copy) NSString *modelPath;
/// inference引擎特殊的配置
@property (nonatomic, strong) id <LiteKitInferenceEngineConfigProtocol> engineConifg;
/// 可以设置自定义的logger
@property (nonatomic, copy) NSString *loggerClassName;

@end
#endif /* LiteKitModelConfigProtocol_h */
