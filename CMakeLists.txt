cmake_minimum_required(VERSION 3.20...3.31)
project("ced" VERSION "0.0.1" LANGUAGES "C")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

file(GLOB SRC_FILES "src/*.c")

add_executable(
  ced

  ${SRC_FILES}
)
