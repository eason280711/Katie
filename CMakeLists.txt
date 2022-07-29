cmake_minimum_required(VERSION 3.7)

project(Katie
        VERSION 0.0.0
        DESCRIPTION "Kaite Engine"
        LANGUAGES CXX
)

# Change the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# config_app.h
set(SCRIPT_ROOT ON)
set(ASSET_ROOT ON)
set(ROOT ON)
configure_file(${CMAKE_SOURCE_DIR}/cmake/config_app.h.cmake ${CMAKE_SOURCE_DIR}/Katie/src/config_app.h)

# find the vsg
if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

find_package(vsg 0.4.0 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(imgui REQUIRED)
find_package(vsgXchange QUIET)

# set the use of C++17 globally as all examples require it
set(CMAKE_CXX_STANDARD 17)

#include

include(Katie/Katie.cmake)
include(Sandbox/Sandbox.cmake)