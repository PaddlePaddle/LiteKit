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


#import <Foundation/Foundation.h>

// @class - LiteKitPredicateMatrix
// @brief - input容器
@interface LiteKitInputMatrix : NSObject
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
