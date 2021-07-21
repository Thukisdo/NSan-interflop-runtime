
cmake_minimum_required(VERSION 3.0)
project(InterflopRuntime)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# Since most backends are header only, we can just include them all and use a single target.
SET(SRC src/Interface.cpp 
        src/backends/DoublePrec.cpp 
        src/Interflop.cpp 
        src/Context.cpp 
        src/Utils.cpp 
        src/backends/MCASync.cpp)

SET(HEADERS include/Flags.hpp 
            include/Backend.hpp 
            include/OpaqueShadow.hpp 
            include/Utils.hpp)

add_library(interflop STATIC ${SRC} ${HEADERS})
target_include_directories(interflop PUBLIC include)

add_library(doubleprec STATIC src/backends/DoublePrec.cpp)
target_include_directories(doubleprec PUBLIC include)

add_library(mcasync STATIC src/backends/MCASync.cpp)
target_include_directories(mcasync PUBLIC include)