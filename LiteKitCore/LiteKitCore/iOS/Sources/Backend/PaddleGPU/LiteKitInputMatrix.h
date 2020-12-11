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
// @brief - input container
@interface LiteKitInputMatrix : NSObject
@property (nonatomic,assign,readonly) int width; //width
@property (nonatomic,assign,readonly) int height;//height
@property (nonatomic,assign,readonly) int channel;
@property (nonatomic,assign,readonly) float* pixels;//input

/*
 * @brief Action = input container
 * @param - width
 * @param - height
 * @param - channel
 * @param - pixels Standardization pixels
 * @return instancetype
 */
-(instancetype)initWithWith:(int)width andHeight:(int)height andChannel:(int)channel andInputPixels:(float *)pixels;

/*
 * @brief Action = input object check available
 * @return BOOL YES:valid NO:invald
 */
-(BOOL) inputValid;
@end
