//
//  MMLInputMatrix.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2017/12/7.
//

#import "MMLInputMatrix.h"
@interface MMLInputMatrix()
@property (nonatomic,assign,readwrite) int width;
@property (nonatomic,assign,readwrite) int height;
@property (nonatomic,assign,readwrite) int channel;
@property (nonatomic,assign,readwrite) float* pixels;
@end
@implementation MMLInputMatrix

-(BOOL) inputValid{
    if(self.width>0 && self.height>0 &&self.pixels>0 && self.channel>0){
        return YES;
    }
    return NO;
}


-(instancetype)initWithWith:(int)width andHeight:(int)height andChannel:(int)channel andInputPixels:(float *)pixels{
    
    if(self = [super init]){
        _width=width;
        _height=height;
        _channel=channel;
        _pixels=pixels;
        
    }
    return self;
}
@end
