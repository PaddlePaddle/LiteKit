//
//  mml_filetools.cpp
//  mml_framework_ios
//
//  Created by Baidu Co.Ltd. on 2020/8/6.
//  Copyright © 2020 Baidu Co.Ltd. All rights reserved.
//

#include "mml_filetools.h"
#include<stdlib.h>

#pragma mark - utils

long get_file_size(FILE *file_handle) {
    //获取当前读取文件的位置 进行保存
    long current_read_position = ftell(file_handle);
    long file_size = 0;
    fseek(file_handle, 0, SEEK_END);
    //获取文件的大小
    file_size = ftell(file_handle);
    //恢复文件原来读取的位置
    fseek(file_handle, current_read_position, SEEK_SET);
    return file_size;
}

char *getFileContent(const char *fileName, size_t *fileSize) {
    // open file
    FILE *aFile = fopen(fileName, "r");
    
    // get file size
    size_t size = get_file_size(aFile);
    if (fileSize != nullptr) {
        *fileSize = size;
    }
    
    // get file content
    char *fileContent = (char *)malloc(sizeof(char)*size);
    fread(fileContent, 1, size, aFile);
    
    // flose file
    fclose(aFile);
    
    // return
    return fileContent;
}
