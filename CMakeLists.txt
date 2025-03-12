cmake_minimum_required(VERSION 3.20...3.31)

project(
  ced
  DESCRIPTION "UNIX text editor"
  VERSION 0.0.1
  LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRCS "./src/*.c")
include_directories(include)

add_executable(
  ced
  ${SRCS}
)
