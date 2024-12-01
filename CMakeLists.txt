cmake_minimum_required(VERSION 3.5.0)
project(P1 VERSION 0.1.0 LANGUAGES C)

include_directories(${PROJECT_SOURCE_DIR}/src/libraries/headers)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")
add_executable(yuniksTerminal ${SOURCES})
