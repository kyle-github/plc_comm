cmake_minimum_required(VERSION 3.7.0)
project(plc_comm_lib VERSION 0.1.0 LANGUAGES C)

# set up includes to get plc_comm.h
include_directories(..)

# set up paths to get included libplctag header and library
include_directories(../../res/include)
# set(CMAKE_PREFIX_PATH "../../res/bin;${CMAKE_PREFIX_PATH}")
# add_library(libplctag STATIC IMPORTED)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions ")

# library
add_library(plc_comm_lib SHARED
            plc_comm.c
            utils.c
            ../../res/bin/libplctag.a
)

# require at least C11
set_property(TARGET plc_comm_lib PROPERTY C_STANDARD 11)

target_compile_options(plc_comm_lib PUBLIC --std=c11)
target_compile_options(plc_comm_lib PUBLIC -fms-extensions)
target_compile_options(plc_comm_lib PUBLIC -Wno-microsoft-anon-tag)

# target_link_options(plc_comm_lib PUBLIC "LINKER:../../res/bin/libplctag.a")

# target_link_libraries(plc_comm_lib libplctag)
