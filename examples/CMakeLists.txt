
project(libstratcom-examples)
cmake_minimum_required(VERSION 3.0)

if(NOT TARGET stratcom)
    # if we are not building as part of libstratcom, we have to find the libstratcom package first
    set(LIBSTRATCOM_PREFIX_PATH "" CACHE PATH "Set this to the installation directory of the libstratcom binaries")

    if(LIBSTRATCOM_PREFIX_PATH)
        list(APPEND CMAKE_PREFIX_PATH ${LIBSTRATCOM_PREFIX_PATH})
    endif()
    find_package(libstratcom NO_MODULE REQUIRED)
endif()

add_executable(led_demo led_demo.c common.h)
target_link_libraries(led_demo stratcom)

add_executable(axis_input axis_input.c common.h)
target_link_libraries(axis_input stratcom)

add_executable(input_events input_events.c common.h)
target_link_libraries(input_events stratcom)

add_executable(tester test.c)
target_link_libraries(tester stratcom)

if(WIN32)
    get_property(dll TARGET stratcom PROPERTY IMPORTED_LOCATION_DEBUG)
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})
    get_property(dll TARGET stratcom PROPERTY IMPORTED_LOCATION_RELEASE)
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR})
endif()
