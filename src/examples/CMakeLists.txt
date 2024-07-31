cmake_minimum_required(VERSION 3.0.0)
project(plc_comm_examples VERSION 0.1.0 LANGUAGES C)

# set up includes to get plc_comm.h
include_directories(../include)

# example executables
add_executable(example1 example1.c)
add_executable(example2 example2.c)
add_executable(example3 example3.c)
add_executable(example4 example4.c)
add_executable(example5 example5.c)
