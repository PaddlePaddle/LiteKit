//
//  MMLMadchine.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2017/11/30.
//

#import "MMLMachine.h"

@interface MMLMachine() {
    BOOL _isCoreMLEnable;
}
@property (nonatomic, strong) NSString *modelURL;

@end
@implementation MMLMachine

-(void)loadModelWithURL:(NSString *)modelURL {
    self.modelURL = modelURL;
}

@end
