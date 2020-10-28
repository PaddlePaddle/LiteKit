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

#if __has_include(<MML/MML.h>)
FOUNDATION_EXPORT double MMLVersionNumber;
FOUNDATION_EXPORT const unsigned char MMLVersionString[];
// you should import all the public headers of your framework using statements like #import <MML/PublicHeader.h>
#import <MML/MMLConvertTools.h>
#import <MML/MMLData.h>

#else
// you should also import all the public headers of your library using statements like #import "PublicHeader.h"
#import "MMLConvertTools.h"
#import "MMLData.h"

#endif
