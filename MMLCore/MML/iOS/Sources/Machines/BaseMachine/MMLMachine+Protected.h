//
//  MMLMachine+Protected.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/1/26.
//

#ifndef MMLMachine_Protected_h
#define MMLMachine_Protected_h

#import <Foundation/Foundation.h>

@interface MMLMachine (Protected)
///machine内model的文件位置
@property (nonatomic, strong) NSString *modelURL;

/**
 * @brief 通过模型位置创建model的方法
 * @param modelURL 模型文件的位置
 */
-(void)loadModelWithURL:(NSString *)modelURL;

@end

#endif /* MMLMachine_Protected_h */
