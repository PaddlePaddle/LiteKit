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


#import "MMLBaseMachine.h"

NS_ASSUME_NONNULL_BEGIN

/// 性能数据
@interface MMLBaseMachine (Performancer)

/// 纯inferenceEngine的加载时间
@property (nonatomic, copy) NSString *loadTime;
/// 纯inferenceEngine的预测时间
@property (nonatomic, copy) NSString *predictTime;
/// 读取paddle的metal资源时间
@property (nonatomic, copy) NSString *readMetalResourceTime;

@end

NS_ASSUME_NONNULL_END
