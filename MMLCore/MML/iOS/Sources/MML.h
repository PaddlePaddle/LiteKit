//
//  MML.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/15.
//  Copyright © 2019 baidu. All rights reserved.
//

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
