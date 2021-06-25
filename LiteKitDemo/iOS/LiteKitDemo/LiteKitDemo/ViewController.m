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

#import "ViewController.h"
#import "HandGestureViewController.h"
#import "SegmentationViewController.h"
#import "SuperResolutionViewController.h"
#import "LiteKitOCRViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"LiteKit";
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self addButtonNamed:@"手势识别" backgroundImage:@"bg_gesture.jpeg" tag:10000 action:@selector(handGestureBtnClickAction:)];
    [self addButtonNamed:@"视频超分" backgroundImage:@"bg_super_resolution.jpeg" tag:10001 action:@selector(superBtnClickAction:)];
    [self addButtonNamed:@"人像分割" backgroundImage:@"bg_portrait.jpeg" tag:10002 action:@selector(segmentationBtnClickAction:)];
    [self addButtonNamed:@"OCR" backgroundImage:@"bg_ocr.jpeg" tag:10003 action:@selector(ocrBtnClickAction:)];
}

- (void)addButtonNamed:(NSString *)name backgroundImage:(NSString *)imageName tag:(NSInteger)tag action:(SEL)selector {
    CGFloat screenWidth = [[UIScreen mainScreen] bounds].size.width;
    CGFloat naviBarHeight = self.navigationController.navigationBar.frame.size.height;
    CGFloat statusBarHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    CGFloat gap = 25;
    CGFloat ratio = 9.0 / 32.0;
    CGFloat width = screenWidth-12*2;
    CGFloat height = ratio * width;
    
    CGFloat y = statusBarHeight+naviBarHeight + gap;
    UIButton *preButton = [self.view viewWithTag:tag-1];
    if (nil != preButton) {
        y = CGRectGetMaxY(preButton.frame) + gap;
    }
    
    CGRect frame = CGRectMake(12, y, width, height);
    UIButton *button = [self createBtn:name frame:frame];
    [button setTag:tag];
    [self.view addSubview:button];
    [button setBackgroundImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
    [button addTarget:self action:selector forControlEvents:UIControlEventTouchUpInside];
}

- (void)handGestureBtnClickAction:(id)sender {
    [self.navigationController pushViewController:[HandGestureViewController new] animated:YES];
}

- (void)superBtnClickAction:(id)sender {
    [self.navigationController pushViewController:[SuperResolutionViewController new] animated:YES];
}

- (void)segmentationBtnClickAction:(id)sender {
    [self.navigationController pushViewController:[SegmentationViewController new] animated:YES];
}

- (void)ocrBtnClickAction:(id)sender {
    [self.navigationController pushViewController:[LiteKitOCRViewController new] animated:YES];
}

- (UIButton*)createBtn:(NSString*)title frame:(CGRect)frame {
    UIButton *button = [[UIButton alloc] initWithFrame:frame];
    button.clipsToBounds = YES;
    button.layer.cornerRadius = 8.f;
    button.layer.shadowRadius = 2.0;
    button.layer.shadowOffset = CGSizeMake(1, 1);
    button.layer.shadowColor = [UIColor blackColor].CGColor;
    button.layer.shadowOpacity = 0.2;
    //
    CGRect rect = button.bounds;
    rect.origin.x = 20;
    rect.size.width -= 20;
    UILabel *label = [[UILabel alloc] initWithFrame:rect];
    label.text = title;
    label.textColor = [UIColor whiteColor];
    label.textAlignment = NSTextAlignmentLeft;
    label.font = [UIFont systemFontOfSize:22.f];
    [button addSubview:label];
    return button;
}

@end
