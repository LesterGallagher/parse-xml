
cmake_minimum_required (VERSION 2.6)

project(xml-parser)

set(CMAKE_C_FLAGS "-Wall -pedantic -Wextra")

set(ALL_SOURCES main.c)
set(ALL_SOURCES ${ALL_SOURCES} xml-parser.c helpers.c)

set(ALL_SOURCES ${ALL_SOURCES} CMemLeak.c)

include_directories("${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}")

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# add_definitions(-D_DEBUG)

add_executable(xml-parser ${ALL_SOURCES})

