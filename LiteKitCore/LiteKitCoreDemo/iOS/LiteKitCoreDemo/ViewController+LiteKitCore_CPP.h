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
