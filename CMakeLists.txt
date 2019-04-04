cmake_minimum_required(VERSION 3.8)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
project(static_enum VERSION "0.1" LANGUAGES CXX)

option(STATIC_ENUM_OPT_BUILD_EXAMPLES "Build static_enum examples" ON)
option(STATIC_ENUM_OPT_BUILD_TESTS "Build and perform static_enum tests" ON)
set(HEADER $<INSTALL_INTERFACE:include/static_enum/static_enum.hpp> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/static_enum/static_enum.hpp>)

add_library(static_enum INTERFACE)
target_sources(static_enum INTERFACE ${HEADER})
target_include_directories(static_enum INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(static_enum INTERFACE cxx_std_17)

if(MSVC)
    add_custom_target(static_enum.header SOURCES ${HEADER})
endif()

if(STATIC_ENUM_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(STATIC_ENUM_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
