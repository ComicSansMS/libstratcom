project(hidapi)

cmake_minimum_required(VERSION 3.0)

if(WIN32)
    set(HIDAPI_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/windows/hid.c
    )
elseif(APPLE)
    set(HIDAPI_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/mac/hid.c
    )
else()
    set(HIDAPI_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/linux/hid.c
    )
endif()

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/hidapi/hidapi.h COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/hidapi ${PROJECT_BINARY_DIR}/hidapi)

add_library(hidapi ${HIDAPI_SOURCE_FILES} ${PROJECT_BINARY_DIR}/hidapi/hidapi.h)
target_include_directories(hidapi PUBLIC ${PROJECT_BINARY_DIR}/hidapi)
