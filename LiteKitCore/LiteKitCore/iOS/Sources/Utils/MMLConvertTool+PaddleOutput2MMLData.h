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
#import "MMLConvertTool.h"
#import "MMLData.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * MMLConvertTool  paddle cpu & gpu output >> MMLData
*/
@interface MMLConvertTool(PaddleOutput2MMLData)

/**
 paddle gpu output >> MMLData

 @param outputData 输入的Paddle GPU output
 @return 转换后的MMLData
*/
+ (MMLData *)convertPaddleGPUOutputToMMLOutput:(PaddleGPUResult *)outputData ;

@end

NS_ASSUME_NONNULL_END
