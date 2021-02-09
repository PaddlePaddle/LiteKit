# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# 添加编译选项
add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Release)
# 减小release 体积
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O3 -DNDEBUG -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O3 -DNDEBUG -fopenmp")

if (ANDROID_ABI STREQUAL "armeabi-v7a")
    set(ABI armeabi-v7a)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -marm -mfpu=neon -mfloat-abi=softfp -funwind-tables -no-canonical-prefixes -O3 -std=c++11 -DNDEBUG  -Wl,--fix-cortex-a8  -fPIE -pie -Wl,--gc-sections -Wl,-z,nocopyreloc")
    include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
elseif (ANDROID_ABI STREQUAL "arm64-v8a")
    set(ABI arm64-v8a)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")
    include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
else ()
    message(FATAL_ERROR "Unkown architecture.")
endif ()

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        litekit_framework
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        ${CMAKE_CURRENT_SOURCE_DIR}/litekit_inference_api.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/paddle_lite/paddle_lite_image_preprocess.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/paddle_lite/paddle_lite_machine_predictor.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library(
        jnigraphics-lib
        jnigraphics
)

### Third Party
SET(paddle_lite_a_path ${PROJECT_SOURCE_DIR}/src/main/jniLibs/${ABI}/libpaddle_api_light_bundled.a)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/paddle_gpu)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/paddle_gpu_header)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/paddle_lite)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/paddle_lite_header)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(litekit_framework PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
#link_directories(${PROJECT_SOURCE_DIR}/src/main/jniLibs)

target_link_libraries( # Specifies the target library.
            litekit_framework
            # Links the log library to the target library.
            ${log-lib}
            ${jnigraphics-lib}
            ${paddle_lite_a_path}
            z
            )