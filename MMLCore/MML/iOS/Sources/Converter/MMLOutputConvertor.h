//
//  MMLOutputConvertor.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/18.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLMachineConfig.h"
#import "MMLData.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * 预测output数据转换MMLData
 */
@interface MMLOutputConvertor : NSObject

/**
  PaddleGPUOutput转换MMLData
 
  @param outputData 输入数据
  @return MMLData 返回的MMLData
 */
+ (MMLData *)convertPaddleGPUOutputToMMLOutput:(id)outputData;


@end

NS_ASSUME_NONNULL_END
