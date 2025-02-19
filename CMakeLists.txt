cmake_minimum_required(VERSION 3.30)
project(algo_combinatoire)

set(CMAKE_CXX_STANDARD 17)

add_executable(algo_combinatoire main.cpp
        originalAbc.cpp
        utils/Agent/Agent.cpp
        utils/Problem/Problem.cpp
        utils/Target/Target.cpp)