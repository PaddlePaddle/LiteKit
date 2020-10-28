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


#import <MML/MML.h>
#import "MMLBaseMachine.h"

NS_ASSUME_NONNULL_BEGIN
///machine
@class PaddleCPU;
@class PaddleCPUConfig;
///input
@class MMLShapedData;
///output
@class MMLData;

@interface MMLPaddleCPUMachine : MMLBaseMachine

/*
 * @brief Action = 初始化模型 模型操作工具Machine
 * @param - modelDir 模型文件路径
 * @param - error 错误容器 （ErrorDomain:MMLPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
 * @return instancetype
 */
- (instancetype)initWithModelDir:(NSString *)modelDir
                       withError:(NSError**)error;

@end

NS_ASSUME_NONNULL_END
