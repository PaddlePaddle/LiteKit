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

#import "HandGestureViewController.h"
#import "HandGestureRecognizeView.h"
#import "HandGestureImageViewController.h"

@interface HandGestureViewController ()
@property (strong, nonatomic) UILabel *timeCost;
@property (nonatomic, strong) LiteKitHandGestureDetector *gestureRecognizer;
@end

@implementation HandGestureViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initView];
    [self createInstance];
}

- (void)initView {
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

#pragma mark - override
- (BaseDetectView*)inferenceContentView {
    HandGestureRecognizeView *detectView = [[HandGestureRecognizeView alloc] init];
    return detectView;
}

- (void)switchImageModelAction:(id)sender {
    [self.navigationController pushViewController:[HandGestureImageViewController new] animated:YES];
}

#pragma mark - delegate

- (void)captureOutput:(AVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection*)connection {
    [self detectWithSampleBuffer:sampleBuffer];
}

- (void)detectWithSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    [self.gestureRecognizer detectWithSampleBuffer:sampleBuffer
                                          complete:^(LiteKitHandGestureDetectResult *result, NSError *error) {
        NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
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

@end
