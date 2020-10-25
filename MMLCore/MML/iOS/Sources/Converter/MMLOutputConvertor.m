//
//  MMLOutputConvertor.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/18.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLOutputConvertor.h"
#import "MMLConvertTools.h"

@implementation MMLOutputConvertor

+ (MMLData *)convertPaddleGPUOutputToMMLOutput:(id)outputData {
    MMLData *returnData = [MMLConvertTool convertPaddleGPUOutputToMMLOutput:outputData];
    return returnData;
}

@end
