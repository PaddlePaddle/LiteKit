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
