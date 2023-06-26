cmake_minimum_required(VERSION 3.0.0)
project(task_Yadro VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(src)