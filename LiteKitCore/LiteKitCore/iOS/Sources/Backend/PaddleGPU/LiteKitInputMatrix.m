/*
 Copyright © 2020 Baidu, Inc. All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#import "LiteKitInputMatrix.h"
@interface LiteKitInputMatrix()
@property (nonatomic,assign,readwrite) int width;
@property (nonatomic,assign,readwrite) int height;
@property (nonatomic,assign,readwrite) int channel;
@property (nonatomic,assign,readwrite) float* pixels;
@end
@implementation LiteKitInputMatrix

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
