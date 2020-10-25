//
//  MMLInputConvertor.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/18.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLInputConvertor.h"
#import "MMLConvertTools.h"
@implementation MMLInputConvertor

+ (MMLInputMatrix *)convertMMLInputToPaddleInput:(MMLData *)inputData {
    return [MMLConvertTool convertMMLInputToPaddleInput:inputData];
}

@end
