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



#import <Foundation/Foundation.h>
#import "PaddleGPU.h"
#import "LiteKitInferenceEngineConfigProtocol.h"

NS_ASSUME_NONNULL_BEGIN

/// Paddle Machine的配置
@interface LiteKitPaddleConfig : NSObject <LiteKitInferenceEngineConfigProtocol>
/// machineType == LiteKitPaddleGPU时，可以配置GPU Inference Engine的配置，此时参数可选
@property (nonatomic, strong) PaddleGPUConfig *paddleGPUConfig;
/// machineType == LiteKitPaddleGPU时，必须配置GPU Inference Engine的net类型，此时参数必选
@property (nonatomic, assign) NetType netType;

/// fluid模型内存指针
@property (nonatomic, assign) unsigned char * modelPointer;
/// fluid参数内存指针
@property (nonatomic, assign) unsigned char * paramPointer;
/// 模型内存大小
@property (nonatomic, assign) size_t modelSize;
/// 参数内存大小
@property (nonatomic, assign) size_t paramSize;
@end

NS_ASSUME_NONNULL_END
