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


#ifndef LiteKitMachine_Protected_h
#define LiteKitMachine_Protected_h

#import <Foundation/Foundation.h>

@interface LiteKitMachine (Protected)
///machine内model的文件位置
@property (nonatomic, strong) NSString *modelURL;

/**
 * @brief 通过模型位置创建model的方法
 * @param modelURL 模型文件的位置
 */
-(void)loadModelWithURL:(NSString *)modelURL;

@end

#endif /* LiteKitMachine_Protected_h */
