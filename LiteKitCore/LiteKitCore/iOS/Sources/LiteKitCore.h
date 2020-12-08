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


#import <Foundation/Foundation.h>

#if __has_include(<LiteKitCore/LiteKitCore.h>)
FOUNDATION_EXPORT double LiteKitVersionNumber;
FOUNDATION_EXPORT const unsigned char LiteKitVersionString[];
// you should import all the public headers of your framework using statements like #import <LiteKitCore/PublicHeader.h>
#import <LiteKitCore/LiteKitConvertTools.h>
#import <LiteKitCore/LiteKitData.h>

#else
// you should also import all the public headers of your library using statements like #import "PublicHeader.h"
#import "LiteKitConvertTools.h"
#import "LiteKitData.h"

#endif
