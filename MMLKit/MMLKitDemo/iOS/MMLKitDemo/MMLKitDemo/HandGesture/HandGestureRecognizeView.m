//
//  HandGestureRecognizeView.m
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import "HandGestureRecognizeView.h"

@implementation HandGestureRecognizeView {
    MMLHandGestureDetectResult *_resultData;
}

- (void)updateResultData:(MMLHandGestureDetectResult *)resultData {
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
    if (_resultData.label != MMLHandGestureDetectionLabelFist) {
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

- (NSString *)labelWithLabelIndex:(MMLHandGestureDetectionLabel)labelIndex {
    switch (labelIndex) {
        case MMLHandGestureDetectionLabelHand:
            return @"hand";
        case MMLHandGestureDetectionLabelFive:
            return @"five";
        case MMLHandGestureDetectionLabelVictory:
            return @"victory";
        case MMLHandGestureDetectionLabelFist:
            return @"fist";
        case MMLHandGestureDetectionLabelOne:
            return @"one";
        case MMLHandGestureDetectionLabelOK:
            return @"ok";
        default:
            return @"";
    }
}

@end
