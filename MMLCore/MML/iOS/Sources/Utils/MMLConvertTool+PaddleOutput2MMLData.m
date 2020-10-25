//
//  MMLConvertTool+PaddleOutput2MMLData.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/27.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLConvertTool+PaddleOutput2MMLData.h"

@implementation MMLConvertTool(PaddleOutput2MMLData)


+ (MMLData *)convertPaddleGPUOutputToMMLOutput:(PaddleGPUResult *)outputData {
    MMLData *returnData = nil;
    
    MMLShapedData *shapedData = [[MMLShapedData alloc] initWithData:outputData.output
                                                          dataSize:outputData.outputSize
                                                              dims:outputData.dim];
    
    returnData = [[MMLData alloc]initWithData:shapedData type:TMMLDataTypeMMLShapedData];
    return returnData;
}


@end
