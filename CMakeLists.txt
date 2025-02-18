cmake_minimum_required(VERSION 3.30)
project(algo_combinatoire)

set(CMAKE_CXX_STANDARD 17)

add_executable(algo_combinatoire main.cpp
        optimizer.cpp
        originalAbc.cpp
        utils/Agent/Agent.cpp
        utils/Problem/Problem.cpp
        utils/Space/BaseVar/BaseVar.cpp
        utils/Space/FloatVar/FloatVar.cpp
        utils/Target/Target.cpp)