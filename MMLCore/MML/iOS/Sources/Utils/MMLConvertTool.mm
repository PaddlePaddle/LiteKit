//
//  MMLConvertTool.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/21.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
