//
//  MMLInputMatrix.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2017/12/7.
//

#import <Foundation/Foundation.h>

// @class - MMLPredicateMatrix
// @brief - input容器
@interface MMLInputMatrix : NSObject
@property (nonatomic,assign,readonly) int width; //宽
@property (nonatomic,assign,readonly) int height;//高
@property (nonatomic,assign,readonly) int channel;
@property (nonatomic,assign,readonly) float* pixels;//input

/*
 * @brief Action = input对象容器
 * @param - width
 * @param - height
 * @param - channel
 * @param - pixels 标准化后的pixels
 * @return instancetype
 */
-(instancetype)initWithWith:(int)width andHeight:(int)height andChannel:(int)channel andInputPixels:(float *)pixels;

/*
 * @brief Action = input对象校验器
 * @return BOOL YES:有效 NO:无效
 */
-(BOOL) inputValid;
@end
