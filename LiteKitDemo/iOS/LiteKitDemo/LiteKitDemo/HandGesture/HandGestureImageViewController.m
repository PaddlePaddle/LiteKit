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

#import "HandGestureImageViewController.h"
#import "HandGestureRecognizeView.h"

@interface HandGestureImageViewController ()
@property (strong, nonatomic) UIImage *image;
@property (strong, nonatomic) UILabel *timeCost;
@property (strong, nonatomic) HandGestureRecognizeView *detectView;
@property (nonatomic, strong) LiteKitHandGestureDetector *gestureRecognizer;
@end

#define ScreenWidth                         [[UIScreen mainScreen] bounds].size.width
#define ScreenHeight                        [[UIScreen mainScreen] bounds].size.height


@implementation HandGestureImageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self initView];
    [self createInstance];
}

- (void)initView {
    // init navi
    self.navigationItem.rightBarButtonItems = @[
        [[UIBarButtonItem alloc] initWithTitle:@"开始识别"
                                         style:UIBarButtonItemStylePlain
                                        target:self
                                        action:@selector(imageDetectAction:)]
    ];
    
    self.image = [UIImage imageNamed:@"test-HandGesture.jpg"];
    CGFloat imgW = self.image.size.width;
    CGFloat imgH = self.image.size.height;
    CGFloat adjustH = imgH * self.view.frame.size.width / imgW;
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, adjustH)];
    imageView.image = self.image;
    imageView.backgroundColor = [UIColor clearColor];
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self.view addSubview:imageView];

    self.detectView = [[HandGestureRecognizeView alloc] initWithFrame:imageView.frame];
    self.detectView.backgroundColor = [UIColor clearColor];
    self.detectView.presetSize = CGSizeMake(imgW, imgH);
    [self.view addSubview:self.detectView];
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12, [self naviBarHeight] + 8, 100, 30)];
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
    self.timeCost = label;
}

- (void)createInstance {
    NSError *error;
    self.gestureRecognizer = [LiteKitHandGestureDetector createHandGestureDetectorWithError:&error];
    if (!self.gestureRecognizer || error) {
        return;
    }
}

- (void)imageDetectAction:(id)sender {
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    [self.gestureRecognizer detectWithUIImage:self.image complete:^(LiteKitHandGestureDetectResult *result, NSError *error) {
        NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
        if (!result || error) {
            return;
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            [self outputDetectResult:result];
            [(HandGestureRecognizeView*)self.detectView updateResultData:result];
            self.timeCost.text = [NSString stringWithFormat:@"%.2fms", (end - begin) * 1000];
        });
    }];
}

- (void)outputDetectResult:(LiteKitHandGestureDetectResult *)result {
    if (result) {
        NSLog(@"Gesture Detect Result:\n"
              "\t\t\t----Detect Label Index:%d\n"
              "\t\t\t----Hand Box Rect: (xStart:%f yStart:%f width:%f height:%f)\n"
              "\t\t\t----Finger Point Position: (x:%f y:%f)",
              (int)result.label,
              result.handBoxRect.origin.x,
              result.handBoxRect.origin.y,
              result.handBoxRect.size.width,
              result.handBoxRect.size.height,
              result.fingerPoint.x,
              result.fingerPoint.y);
    }
}

- (float)naviBarHeight {
    return self.navigationController.navigationBar.frame.size.height + [[UIApplication sharedApplication] statusBarFrame].size.height;
}
@end
