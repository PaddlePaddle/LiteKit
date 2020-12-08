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
