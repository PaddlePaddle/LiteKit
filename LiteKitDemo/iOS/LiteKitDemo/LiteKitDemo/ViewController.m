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

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"LiteKit";
    self.view.backgroundColor = [UIColor whiteColor];
    
    CGFloat screenWidth = [[UIScreen mainScreen] bounds].size.width;
    CGFloat naviBarHeight = self.navigationController.navigationBar.frame.size.height;
    CGFloat statusBarHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    CGFloat gap = 25;
    CGFloat ratio = 9.0 / 32.0;
    CGFloat width = screenWidth-12*2;
    CGFloat height = ratio * width;
    
    CGRect frame = CGRectMake(12, statusBarHeight+naviBarHeight + gap, width, height);
    UIButton *btnHand = [self createBtn:@"手势识别" frame:frame];
    [self.view addSubview:btnHand];
    [btnHand setBackgroundImage:[UIImage imageNamed:@"bg_gesture.jpeg"] forState:UIControlStateNormal];
    [btnHand addTarget:self action:@selector(handGestureBtnClickAction:) forControlEvents:UIControlEventTouchUpInside];
    
    frame = CGRectMake(12, CGRectGetMaxY(btnHand.frame) + gap, width, height);
    UIButton *btnSuper = [self createBtn:@"视频超分" frame:frame];
    [self.view addSubview:btnSuper];
    [btnSuper setBackgroundImage:[UIImage imageNamed:@"bg_super_resolution.jpeg"] forState:UIControlStateNormal];
    [btnSuper addTarget:self action:@selector(superBtnClickAction:) forControlEvents:UIControlEventTouchUpInside];
    
    frame = CGRectMake(12, CGRectGetMaxY(btnSuper.frame) + gap, width, height);
    UIButton *btnPortrait = [self createBtn:@"人像分割" frame:frame];
    [self.view addSubview:btnPortrait];
    [btnPortrait setBackgroundImage:[UIImage imageNamed:@"bg_portrait.jpeg"] forState:UIControlStateNormal];
    [btnPortrait addTarget:self action:@selector(segmentationBtnClickAction:) forControlEvents:UIControlEventTouchUpInside];
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
