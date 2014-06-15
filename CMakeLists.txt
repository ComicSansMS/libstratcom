
project(libstratcom)
cmake_minimum_required(VERSION 3.0)

set(HIDAPI_BINARY_DIR ${PROJECT_BINARY_DIR}/hidapi_build)

find_package(Git REQUIRED)
include(ExternalProject)
ExternalProject_Add(hidapi
    GIT_REPOSITORY https://github.com/signal11/hidapi.git
    GIT_TAG hidapi-0.8.0-rc1
    UPDATE_COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/external/hidapi/CMakeLists.txt .
    BINARY_DIR ${HIDAPI_BINARY_DIR}
    INSTALL_COMMAND ""
    LOG_BUILD 1
    LOG_TEST 1
    LOG_INSTALL 1
)

set(LIBSTRATCOM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIBSTRATCOM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(LIBSTRATCOM_SOURCE_FILES
    ${LIBSTRATCOM_SOURCE_DIR}/stratcom.cpp
)

set(LIBSTRATCOM_HEADER_FILES
    ${LIBSTRATCOM_INCLUDE_DIR}/stratcom.h
)

source_group(include FILES ${LIBSTRATCOM_HEADER_FILES})

add_library(stratcom SHARED ${LIBSTRATCOM_SOURCE_FILES} ${LIBSTRATCOM_HEADER_FILES})
add_dependencies(stratcom hidapi)
if(MSVC)
    target_link_libraries(stratcom LINK_PRIVATE ${HIDAPI_BINARY_DIR}/$<CONFIG>/hidapi.lib setupapi.lib)
else()
    if(APPLE)
        target_link_libraries(stratcom LINK_PRIVATE ${HIDAPI_BINARY_DIR}/libhidapi.a "-framework IOKit" "-framework CoreFoundation")
    endif()
endif()
target_compile_definitions(stratcom PRIVATE LIBSTRATCOM_EXPORT)
if(MSVC)
    target_compile_options(stratcom PRIVATE /W4)
else()
    target_compile_options(stratcom PRIVATE -pedantic -Wall -std=c++11)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(stratcom PRIVATE -stdlib=libc++)
    endif()
endif()
target_include_directories(stratcom PRIVATE ${HIDAPI_BINARY_DIR}/hidapi)
target_include_directories(stratcom PUBLIC ${LIBSTRATCOM_INCLUDE_DIR})

add_executable(tester src/test.c)
target_link_libraries(tester stratcom)
