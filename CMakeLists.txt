
cmake_minimum_required(VERSION 3.0)
project(InterflopRuntime)

# FIXME : we probaably shouldn't alwaus build with mavx512f 
# This removes compilers warning about changes in the abi
# But may cause incompatibility on other platforms
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

add_library(interflop-utils STATIC "include/Utils.hpp" "src/Utils.cpp")
set_target_properties(interflop-utils PROPERTIES PUBLIC_HEADERS "include/Utils.hpp")
target_include_directories(interflop-utils PUBLIC include)


add_library(interflop-dummy-utils STATIC "include/Utils.hpp" "src/Utils.cpp")
target_compile_definitions(interflop-dummy-utils PUBLIC -DDUMMY_NSAN_INTERFACE)
set_target_properties(interflop-dummy-utils PROPERTIES PUBLIC_HEADERS "include/Utils.hpp")
target_include_directories(interflop-dummy-utils PUBLIC include)

SET(SRC src/Interface.cpp 
        src/Interflop.cpp 
        src/Context.cpp 
)

SET(HEADERS include/Flags.hpp 
            include/Backend.hpp 
            include/OpaqueShadow.hpp
            include/Context.hpp 
)

add_library(interflop-core STATIC ${SRC} ${HEADERS})
target_include_directories(interflop-core PUBLIC include)

add_library(interflop-doubleprec STATIC "src/backends/DoublePrec.cpp"
                                        "include/Context.hpp")
target_include_directories(interflop-doubleprec PUBLIC include)


add_library(interflop-mcasync STATIC "src/backends/MCASync.cpp"
                                     "include/Context.hpp")
target_include_directories(interflop-mcasync PUBLIC include)

set_target_properties(interflop-core interflop-utils interflop-mcasync interflop-doubleprec interflop-dummy-utils
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Taken from google-test setup example
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/96f4ce02a3a78d63981c67acbd368945d11d7d70.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory(tests)
