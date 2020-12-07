
cmake_minimum_required(VERSION 3.0.0)
project(litekit_c_ios_arch)

# 添加编译选项
add_definitions(-std=c++11)

# set variable
set(CMAKE_BUILD_TYPE Release)
SET(PROJECT_SOURCE_DIR ../)
SET(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
SET(IS_IOS 1)

## build flags
set(CMAKE_CXX_FLAGS "-O3 -s -DNDEBUG ${CMAKE_CXX_FLAGS} -Wno-attributes")
if(IS_IOS)
    set(CMAKE_CXX_FLAGS "-mfpu=neon -marm -fobjc-abi-version=2 -fobjc-arc \
        -std=gnu++11 -stdlib=libc++ -isysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")
    add_compile_options(-fembed-bitcode)
else()
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "litekit______not support architecture.")
endif()


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

########### 仅含paddle_lite的构建  start#######
add_library( # Sets the name of the library.
             litekit_framework_ios
             # Sets the library as a shared library. 
             STATIC
             # Provides a relative path to your source file(s).
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/litekit_inference_api.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/common_log.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/litekit_convertor.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/litekit_filetools.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/paddle_gpu/paddle_gpu_machine_predictor.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/paddle_lite/paddle_lite_machine_predictor.cpp
             ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/paddle_lite/paddle_lite_image_preprocess.cpp
            )
            
########### 仅含paddle_lite的构建  end#######  
          
### Third Party

# Paddle Lite 
add_library(paddle_lite SHARED IMPORTED)
set_target_properties(paddle_lite PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/src/main/iOSLibs/paddle_api_light_bundled/libpaddle_api_light_bundled.a)
set(PROJ_DEPS ${PROJ_DEPS} paddle_lite)


## 添加头文件目录
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/paddle_gpu_header )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/cpp/litekit_framework/paddle-lite-header )


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

set_target_properties(litekit_framework_ios PROPERTIES RUNTIME_OUTPUT_DIRECTORY> "${RUNTIME_OUTPUT_DIRECTORY}/lib")


