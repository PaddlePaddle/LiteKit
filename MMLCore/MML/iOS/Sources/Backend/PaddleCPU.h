//
//  PaddleCPUOC.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2020/2/12.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class PaddleCPUConfig;
@class PaddleCPUResult;
@class PaddleCPUInput;
@interface PaddleCPU : NSObject


/**
 @b 创建对象

 @param config 配置
 @return PaddleCPU对象
 */
- (instancetype)initWithConfig:(PaddleCPUConfig *)config;

/**
 @b 加载machine
 
 @param error 错误信息
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error;


/**
 @b 进行预测, 预处理 means 值为 0, scale 值为 1

 @param input 输入图像
 @param error 返回错误
 @return 预测结果
 */
- (PaddleCPUResult *)predictWithInput:(PaddleCPUInput *)input
                                 error:(NSError **) error;


@end


#pragma mark - @Class PaddleCPUConfig
@interface  PaddleCPUConfig: NSObject
@property (nonatomic, strong) NSString *modelDir;

@end



#pragma mark - @Class PaddleCPUShapedData
@interface  PaddleCPUShapedData: NSObject
///数据float*
@property (nonatomic, readonly) float *data;
///数据size
@property (nonatomic, readonly) NSInteger dataSize;
///数据dims
@property (nonatomic, readonly) NSArray <NSNumber *> *dim;

/**
 * 初始化方法
 
 @param data 初始化数据
 @param dataSize 数据长度
 @param dim 数据维度
 @return 初始化的数据
 */
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim;
@end

#pragma mark - @Class PaddleCPUResult
@interface  PaddleCPUResult: PaddleCPUShapedData

@end

#pragma mark - @Class PaddleCPUInput
@interface  PaddleCPUInput: PaddleCPUShapedData

@end

NS_ASSUME_NONNULL_END
