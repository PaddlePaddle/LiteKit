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

#import "HandGestureRecognizeView.h"

@implementation HandGestureRecognizeView {
    LiteKitHandGestureDetectResult *_resultData;
}

- (void)updateResultData:(LiteKitHandGestureDetectResult *)resultData {
    _resultData = resultData;
    [self setNeedsDisplay];
}

-(void)layoutSubviews {
    [super layoutSubviews];
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    
    if (!_resultData) {
        return;
    }

    float scale = self.frame.size.width / self.presetSize.width;
    if (self.frame.size.width > self.frame.size.height) {
        scale = self.frame.size.height / self.presetSize.width;
    }
    float width = _resultData.handBoxRect.size.width * scale;
    float height = _resultData.handBoxRect.size.height * scale;
    float xStart = _resultData.handBoxRect.origin.x * scale;
    float yStart = _resultData.handBoxRect.origin.y * scale;
    //draw detect rect
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextClearRect(context, rect);
    CGContextSetRGBStrokeColor(context, 0.0, 1.0, 0.0, 1.0);
    CGContextSetLineWidth(context, 2);
    CGRect handBox = CGRectMake(xStart, yStart, width, height);
    CGContextAddRect(context, handBox);
    CGContextStrokePath(context);
    // draw finger point if detection result is not fist
    if (_resultData.label != LiteKitHandGestureDetectionLabelFist) {
        CGContextSetRGBStrokeColor(context, 1.0, 1.0, 0.0, 1.0);
        CGContextSetLineWidth(context, 2);
        CGContextAddArc(context, _resultData.fingerPoint.x * scale, _resultData.fingerPoint.y * scale, 2, 0, 2 * 3.14, 0);
        CGContextStrokePath(context);
    }
    // draw label and confidence
    NSString *label = [self labelWithLabelIndex:_resultData.label];
    NSString *string = [NSString stringWithFormat:@"%@: %f", label, _resultData.confidence];
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    attributes[NSFontAttributeName] = [UIFont systemFontOfSize:16];
    attributes[NSForegroundColorAttributeName] = [UIColor redColor];
    attributes[NSStrokeColorAttributeName] = [UIColor redColor];
    attributes[NSStrokeWidthAttributeName] = @(-5);
    CGPoint textPosition = CGPointMake(xStart, yStart - 20);
    [string drawAtPoint:textPosition withAttributes:attributes];
    _resultData = nil;
}

- (NSString *)labelWithLabelIndex:(LiteKitHandGestureDetectionLabel)labelIndex {
    switch (labelIndex) {
        case LiteKitHandGestureDetectionLabelHand:
            return @"hand";
        case LiteKitHandGestureDetectionLabelFive:
            return @"five";
        case LiteKitHandGestureDetectionLabelVictory:
            return @"victory";
        case LiteKitHandGestureDetectionLabelFist:
            return @"fist";
        case LiteKitHandGestureDetectionLabelOne:
            return @"one";
        case LiteKitHandGestureDetectionLabelOK:
            return @"ok";
        default:
            return @"";
    }
}

@end
