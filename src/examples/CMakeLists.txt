cmake_minimum_required(VERSION 3.0.0)
project(plc_comm_examples VERSION 0.1.0 LANGUAGES C)

# set up includes to get plc_comm.h
include_directories(../lib/include)

# example executables
add_executable(simple_single_tag_sync simple_single_tag_sync.c)
add_executable(simple_single_tag_async simple_single_tag_async.c)
add_executable(simple_multiple_tag_sync simple_multiple_tag_async.c)

