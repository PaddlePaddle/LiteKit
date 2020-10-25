//
//  MMLConvertTool+PaddleOutput2MMLData.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/27.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
