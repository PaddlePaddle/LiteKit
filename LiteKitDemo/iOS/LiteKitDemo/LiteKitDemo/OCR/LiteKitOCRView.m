//
//  LiteKitOCRView.m
//  LiteKitOCRDemo
//
//  Created by Wang,Zhiyong on 2021/6/15.
//  Copyright © 2021 wangzhiyong04. All rights reserved.
//

#import "LiteKitOCRView.h"

@implementation LiteKitOCRView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(void)setResult:(NSArray<LiteKitOCRResult *> *)result {
    if (result != self.result) {
        _result = [result copy];
        [self setNeedsDisplay];
    }
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    
    if (!_result) {
        return;
    }

    float scale = self.frame.size.width / self.presetSize.width;

    CGContextRef context = UIGraphicsGetCurrentContext();    
    for (LiteKitOCRResult *aResult in _result) {
        for (int i=0; i<aResult.points.count; i++) {
            CGPoint point = [aResult pointAtIndex:i%4];
            CGPoint pointNext = [aResult pointAtIndex:(i+1)%4];
            CGContextSetLineWidth(context, 2.0);
            // CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
            CGContextSetRGBStrokeColor(context, 0.0, 0.0, 0.0, 1.0);
            CGContextSetLineDash(context, 1, (CGFloat[]){3, 1}, 1);
            CGContextMoveToPoint(context, point.x*scale, point.y*scale);
            CGContextAddLineToPoint(context, pointNext.x*scale, pointNext.y*scale);
            CGContextStrokePath(context);
        }
        
        
        CGPoint point0 = [aResult pointAtIndex:0];
        NSString *string = aResult.label;
        NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
        attributes[NSFontAttributeName] = [UIFont systemFontOfSize:10];
        attributes[NSForegroundColorAttributeName] = [UIColor redColor];
        attributes[NSStrokeColorAttributeName] = [UIColor redColor];
        attributes[NSStrokeWidthAttributeName] = @(-5);
        CGPoint textPosition = CGPointMake(point0.x*scale, point0.y*scale - 5.0);
        [string drawAtPoint:textPosition withAttributes:attributes];
    }
}
@end
