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
#import <LiteKitCore/LiteKitPaddleConfig.h>
#import <LiteKitCore/LiteKitMachineConfig.h>
#import <LiteKitCore/LiteKitMachineService.h>
#import <LiteKitCore/LiteKitDataProcess.h>
#import <LiteKitCore/LiteKitLogger.h>
#import <LiteKitCore/LiteKitCore.h>

NS_ASSUME_NONNULL_BEGIN

static const int kLiteKitInputBatch      = 1;
static const int kLiteKitInputChannel    = 3;
static const int kLiteKitInputHeight     = 224;
static const int kLiteKitInputWidth      = 224;

@interface ViewController(LiteKitCore_OC)

- (LiteKitBaseMachine *)loadLiteKitWithModelDir_GPU:(NSString *)modelDir;
- (LiteKitBaseMachine *)loadLiteKitWithModelDir_CPU:(NSString *)modelDir;

@end

NS_ASSUME_NONNULL_END
