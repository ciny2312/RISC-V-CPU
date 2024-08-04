cmake_minimum_required(VERSION 3.16)
project(Simulator)

set(CMAKE_CXX_STANDARD 23)

include_directories(src/include)

add_executable(code main.cpp alu/alu.cpp decoder/decoder.cpp decoder/func.cpp LSB/lsb.cpp memory/memory.cpp ROB/rob.cpp RS/rs.cpp )