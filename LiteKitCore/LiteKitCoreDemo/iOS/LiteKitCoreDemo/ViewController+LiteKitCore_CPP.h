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

///litekit  header
#import "litekit_inference_api.h"

NS_ASSUME_NONNULL_BEGIN

@interface ViewController(LiteKitCore_CPP)

/// CPU
- (litekit_framework::LiteKitMachineService *)loadLiteKitWithModelDir_LITE_MODEL_FROM_BUFFER:(NSString *)modelDir;
- (litekit_framework::LiteKitMachineService *)loadLiteKitWithModelDir_LITE_MODEL_FROM_FILE:(NSString *)modelDir;

- (std::shared_ptr<litekit_framework::LiteKitMachineService>)loadLiteKitWithModelDir_Shared:(NSString *)modelDir;

/// GPU
- (std::shared_ptr<litekit_framework::LiteKitMachineService>)loadLiteKitWithModelDir_GPU_CPP:(NSString *)modelDir;
@end

NS_ASSUME_NONNULL_END
