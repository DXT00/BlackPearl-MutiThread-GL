cmake_minimum_required(VERSION 3.10)
project(BlackPearl-MultiThread-GL LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 静态链接运行时库
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded" CACHE STRING "" FORCE)



if(ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/arm64-v8a")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../bin/arm64-v8a")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../bin/arm64-v8a")
    set(CMAKE_OBJECT_OUTPUT_DIRECTORY "../bin/bin-int/arm64-v8a")
else()
    # 输出目录和中间目录
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")
    set(CMAKE_OBJECT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int/${CMAKE_CFG_INTDIR}")
endif ()


message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY---${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_CFG_INTDIR---${CMAKE_CFG_INTDIR}")

# 添加源文件
file(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES false "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# 创建静态库目标
add_executable(BlackPearl-MultiThread-GL ${SRC_FILES})

# 宏定义
add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -DGLFW_INCLUDE_NONE

)

# 头文件包含目录
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        # "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
        # 注意：下面的路径需要根据实际情况调整，特别是那些使用了占位符的路径
        # "${CMAKE_CURRENT_SOURCE_DIR}/../%{IncludeDir.GLFW}"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/directxtex_uwp.2022.7.30.1/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLEW/include"

)
# 条件判断
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0")
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")

elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")
    message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
else()
    message("else:${CMAKE_BUILD_TYPE}")
    message("else:${CMAKE_C_FLAGS_RELEASE}")
endif()

# 库目录
target_link_directories(
        BlackPearl-MultiThread-GL
        PRIVATE 
        #${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/bin/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ImGui
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/bin/Debug
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp/bin/Debug
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/bin/Debug
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLEW/bin/Debug
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/bin/Debug


)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/bin/Debug")

# 定义一个选项，但暂时不指定其值
option(RENDER_API "Select render api type (default: opengl)" "opengl")

# 设置默认值
set(RENDER_API "opengl" CACHE STRING "Select render api type (direct3d, vulkan, opengl)")

#set(RENDER_API "opengl")
# 链接库和条件编译
if(${RENDER_API} STREQUAL "opengl")
    message("RENDER_API = opengl")
#     target_link_libraries(
#             BlackPearl-MultiThread-GL
#             PRIVATE
#         #     ImGui
#             spdlog
#     )
    if(WIN32)
        target_link_libraries(
                BlackPearl-MultiThread-GL
                PRIVATE
                GLFW
                GLEW
                Glad
                opengl32

        )
    elseif(ANDROID)
        target_link_libraries(
            BlackPearl-MultiThread-GL
            PRIVATE
                EGL
                GLESv3
                assimp
    )
    endif ()
    add_definitions(
            -DGLFW_INCLUDE_NONE
            -DGE_API_OPENGL
    )
elseif(${RENDER_API} STREQUAL "vulkan")
    message("RENDER_API = vulkan")
    target_link_libraries(
            BlackPearl-MultiThread-GL
            PRIVATE
            GLFW
            GLEW
            Glad
            ImGui
            vulkan-1
            assimp-vc142-mtd
            spdlog
    )
    add_definitions(
            -DGLFW_INCLUDE_NONE
            -DGE_API_VULKAN
    )
elseif(${RENDER_API} STREQUAL "direct3d")
    message("RENDER_API = direct3d")
    target_link_libraries(
            BlackPearl-MultiThread-GL
            PRIVATE
            GLFW
            GLEW
            Glad
            ImGui
            assimp-vc142-mtd
    )
    add_definitions(
            -DGLFW_INCLUDE_NONE
            -DGE_API_D3D12
    )
else() #default opengl
    message("set default RENDER_API = opengl")
    target_link_libraries(
            BlackPearl-MultiThread-GL
            PRIVATE
            GLFW
            GLEW
            Glad
            #ImGui
            opengl32
            assimp-vc142-mtd
            spdlog
    )
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
            # 其他Windows平台宏...
    )
elseif(ANDROID)
    message(STATUS "Building for Android")
    add_definitions(
            -DGE_PLATFORM_ANDRIOD
            -DGLEW_NO_GLU
            # 其他Windows平台宏...
    )
    # 在此处添加Android平台特有的配置
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
target_precompile_headers(BlackPearl-MultiThread-GL PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
set(PRECOMPILED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.cpp")