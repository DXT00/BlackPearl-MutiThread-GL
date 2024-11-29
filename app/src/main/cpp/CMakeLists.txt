# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("blackpearl_multithread_gl_andriod")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
message(STATUS "CMAKE_PROJECT_NAME-------${CMAKE_PROJECT_NAME}")
#target_link_libraries(${CMAKE_PROJECT_NAME}
#        # List libraries link to the target library
#        android
#        log)


set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/BlackPearl-MultiThread-GL")
if(WIN32)
    set(ARCH "win64")
elseif(ANDROID)
    set(ARCH "arm64-v8a")
    message(STATUS "building andriod")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${ARCH}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${ARCH}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${ARCH}")


# 添加源文件
file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false "${SRC_DIR}/*.cpp" "${SRC_DIR}/src/*.h")

# 创建动态库目标
#add_executable(BlackPearl-MultiThread-GL ${SRC_FILES})
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        ${SRC_FILES}
        ../../../../../BlackPearl-MutiThread-GL/src/pch.h
)

# 宏定义
add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -DGLFW_INCLUDE_NONE

)

# 头文件包含目录
include_directories(
        "${SRC_DIR}"
        # "${VENDOR_DIR}/spdlog/include"
        # 注意：下面的路径需要根据实际情况调整，特别是那些使用了占位符的路径

        "${VENDOR_DIR}"
        "${VENDOR_DIR}/glm"
        "${VENDOR_DIR}/stb"

)
if(WIN32)
    target_include_directories(
            "${VENDOR_DIR}/Glad/include"
            "${VENDOR_DIR}/GLFW/include"
            "${VENDOR_DIR}/directxtex_uwp.2022.7.30.1/include"
            "${VENDOR_DIR}/GLEW/include"
    )
elseif(ANDROID)
    include_directories(
            "${VENDOR_DIR}/GLESv3/include"
    )
    message(STATUS "gles = ------------${VENDOR_DIR}/GLESv3/include")
endif()

# 库目录
target_link_directories(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        ${VENDOR_DIR}/spdlog/bin/Debug/${ARCH}
        ${VENDOR_DIR}/assimp/bin/Debug/${ARCH}
       # ${VENDOR_DIR}/GLESv3/bin/Debug/${ARCH}
)
if(WIN32)
    target_link_directories(
            ${CMAKE_PROJECT_NAME}
            PRIVATE
            ${VENDOR_DIR}/GLFW/bin/Debug/${ARCH}
            ${VENDOR_DIR}/GLEW/bin/Debug/${ARCH}
            ${VENDOR_DIR}/Glad/bin/Debug/${ARCH}
    )
elseif(ANDROID)
    target_link_directories(
            ${CMAKE_PROJECT_NAME}
            PRIVATE
            ${VENDOR_DIR}/GLESv3/bin/Debug/${ARCH}
    )
endif()

# 定义一个选项，但暂时不指定其值
option(RENDER_API "Select render api type (default: opengl)" "opengl")

# 设置默认值
set(RENDER_API "opengl" CACHE STRING "Select render api type (direct3d, vulkan, opengl)")

set(RENDER_API "opengl")
# 链接库和条件编译
if(${RENDER_API} STREQUAL "opengl")
    message("RENDER_API = opengl")
    if(WIN32)
        target_link_libraries(
                ${CMAKE_PROJECT_NAME}
                PRIVATE
                GLFW
                GLEW
                Glad
                opengl32
        )
    elseif(ANDROID)
        target_link_libraries(
                ${CMAKE_PROJECT_NAME}
                PRIVATE
                EGL
                GLESv3
                #assimp
                log
                android
        )
    endif ()
    add_definitions(
            -DGLFW_INCLUDE_NONE
            -DGE_API_OPENGL
    )
endif()

# Windows平台特有设置
if(WIN32)
    message(STATUS "Building for Windows")
    add_definitions(
            -DGE_PLATFORM_WINDOWS
    )
elseif(ANDROID)
    message(STATUS "Building for Android")
    add_definitions(
            -DGE_PLATFORM_ANDRIOD
            -DGLEW_NO_GLU
    )
endif()

# 构建配置设置
# 设置Windows系统版本，适用于Visual Studio Generator
if(WIN32)
    set(CMAKE_SYSTEM_VERSION "10.0") # Windows 10
endif()

# 定义构建类型
set(BUILD_TYPES Debug Release MinSizeRel)
foreach(BUILD_TYPE ${BUILD_TYPES})
    string(TOUPPER ${BUILD_TYPE} BUILD_TYPE_UC)

    # 设置CMake构建类型
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting CMAKE_BUILD_TYPE to ${BUILD_TYPE_UC}")
        set(CMAKE_BUILD_TYPE ${BUILD_TYPE} CACHE STRING "Build type" FORCE)
    endif()

    # 根据构建类型设置编译选项
    if(BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Configuring for Debug build")
        if(WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /Zi") # Disable optimizations, enable debug info
        endif ()
    elseif(BUILD_TYPE STREQUAL "Release")
        message(STATUS "Configuring for Release build")
        if(WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2") # Optimize for speed
        endif ()
    elseif(BUILD_TYPE STREQUAL "MinSizeRel")
        message(STATUS "Configuring for MinSizeRel build")
    endif()
endforeach()



# 预编译头
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE "${SRC_DIR}/pch.h")
set(PRECOMPILED_HEADER "${SRC_DIR}/pch.cpp")