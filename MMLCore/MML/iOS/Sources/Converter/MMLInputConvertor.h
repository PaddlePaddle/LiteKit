//
//  MMLInputConvertor.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/18.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLMachineConfig.h"
#import "MMLData.h"
#import "MMLInputMatrix.h"

NS_ASSUME_NONNULL_BEGIN

/**
 *MMLInputConvertor
 */
@interface MMLInputConvertor : NSObject

/**
  MMLData转换PaddleInput
 
  @param inputData 输入数据
  @return MMLInputMatrix返回的paddle input matrix
 */
+ (MMLInputMatrix *)convertMMLInputToPaddleInput:(MMLData *)inputData;


@end

NS_ASSUME_NONNULL_END
