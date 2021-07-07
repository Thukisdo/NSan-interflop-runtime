
cmake_minimum_required(VERSION 3.0)
project(InterflopRuntime)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")


SET(SRC src/Interface.cpp src/runtimes/DoublePrec.cpp src/Interflop.cpp)
SET(HEADERS include/runtimes/DoublePrec.hpp include/runtimes/MCASync.hpp include/Flags.hpp include/Runtime.hpp include/Shadow.hpp include/Utils.hpp)

add_library(interflop STATIC ${SRC} ${HEADERS})
target_include_directories(interflop PUBLIC include)
