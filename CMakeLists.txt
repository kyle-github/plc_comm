cmake_minimum_required(VERSION 3.0.0)
project(plc_comm VERSION 0.1.0 LANGUAGES C)

# add_library(plc_comm src/plc_comm.c)

# examples
add_executable(simple_single_tag_sync src/include/plc_comm.h src/examples/simple_single_tag_sync.c)
add_executable(simple_single_tag_async src/include/plc_comm.h src/examples/simple_single_tag_async.c)
add_executable(simple_multiple_tag_sync src/include/plc_comm.h src/examples/simple_multiple_tag_async.c)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
