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
#import "LiteKitData.h"
#import "LiteKitMachineConfig.h"
#import "LiteKitInputMatrix.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * LiteKitConvertTool LiteKitdata <--> Machine input\Machine output转换工具
*/
@interface LiteKitConvertTool : NSObject

#pragma mark input

/**
 LiteKitData -> LiteKitInputMatrix转换

 @param inputData 输入的LiteKitData
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)convertLiteKitInputToPaddleInput:(LiteKitData *)inputData;


@end

NS_ASSUME_NONNULL_END
