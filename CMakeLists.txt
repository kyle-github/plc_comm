cmake_minimum_required(VERSION 3.15.0)
project(plc_comm VERSION 0.1.0 LANGUAGES C)

if("${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
    set(TOOLCHAIN_FILE "${CMAKE_SYSTEM_NAME}-${CMAKE_C_COMPILER_ID}.cmake")
endif()

add_subdirectory(src/lib)
# add_subdirectory(src/examples)
