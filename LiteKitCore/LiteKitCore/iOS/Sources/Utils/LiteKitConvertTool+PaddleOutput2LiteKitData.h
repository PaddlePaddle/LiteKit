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
#import "LiteKitConvertTool.h"
#import "LiteKitData.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * LiteKitConvertTool  paddle cpu & gpu output >> LiteKitData
*/
@interface LiteKitConvertTool(PaddleOutput2LiteKitData)

/**
 paddle gpu output >> LiteKitData

 @param outputData 输入的Paddle GPU output
 @return 转换后的LiteKitData
*/
+ (LiteKitData *)convertPaddleGPUOutputToLiteKitOutput:(PaddleGPUResult *)outputData ;

@end

NS_ASSUME_NONNULL_END
