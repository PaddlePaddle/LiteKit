// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.



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
