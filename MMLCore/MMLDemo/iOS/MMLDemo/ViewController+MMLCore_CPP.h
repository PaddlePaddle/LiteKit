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

#import "ViewController.h"

///mml  header
#import "mml_inference_api.h"

NS_ASSUME_NONNULL_BEGIN

@interface ViewController(MMLCore_CPP)

/// CPU
- (mml_framework::MMLMachineService *)loadMMLWithModelDir_LITE_MODEL_FROM_BUFFER:(NSString *)modelDir;
- (mml_framework::MMLMachineService *)loadMMLWithModelDir_LITE_MODEL_FROM_FILE:(NSString *)modelDir;

- (std::shared_ptr<mml_framework::MMLMachineService>)loadMMLWithModelDir_Shared:(NSString *)modelDir;

/// GPU
- (std::shared_ptr<mml_framework::MMLMachineService>)loadMMLWithModelDir_GPU_CPP:(NSString *)modelDir;
@end

NS_ASSUME_NONNULL_END
