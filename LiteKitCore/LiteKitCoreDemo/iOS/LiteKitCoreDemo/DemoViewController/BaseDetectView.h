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

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseDetectView : UIView

@property (nonatomic, assign) float offsetY; // 子控件布局的起始位置
@property (nonatomic, assign) CGSize presetSize;// 图像分辨率

@end

NS_ASSUME_NONNULL_END
