cmake_minimum_required(VERSION 3.0.0)
project(shr_mem_reader)

set(CXX_STANDARD 17)
set(target_name shr_mem_reader)

add_executable(${target_name} main.cpp shr_mem_reader.h shr_mem_reader.cpp)

target_link_libraries(${target_name} rt)