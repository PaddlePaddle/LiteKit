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


#import "MMLConvertTool.h"
#import "MMLConvertTools.h"

@implementation MMLConvertTool

#pragma mark input
+ (MMLInputMatrix *)convertMMLInputToPaddleInput:(MMLData *)inputData {
    MMLInputMatrix *returnData = nil;
    switch (inputData.type) {
        case TMMLDataTypeImage:
            returnData = [self inputMatrixConvertFromImage:inputData.image];
            break;
        case TMMLDataTypeImageURL:
            returnData = [self inputMatrixConvertFromImageURL:inputData.imageURL];
            break;
        case TMMLDataTypeCVPixelBuffer:
            returnData = [self inputMatrixConvertFromCVPixelBuffer:inputData.pixelBuffer];
            break;
        case TMMLDataTypeMultiArray:
            returnData = [self inputMatrixConvertFromMultiArray:inputData.multiArray];
            break;
        case TMMLDataTypeMMLShapedData:
            returnData = [self inputMatrixConvertFromMMLShapedData:inputData.mmlShapedData];
            break;
        default:
            break;
    }
    return returnData;
}

@end
