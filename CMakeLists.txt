
cmake_minimum_required(VERSION 3.0)
project(InterflopRuntime)

# FIXME : we probably shouldn't always build with mavx
# This removes compilers warning about changes in the abi
# But may cause incompatibility on other platforms
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -std=c++17")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

# Automatically generate the interface
# Will output to buildir, source dir will not be modified
add_custom_command(
  OUTPUT Interface.cpp
  # Add dependency on the script to prevent multiple builds
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/InterfaceGenerator.py
  COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/src/InterfaceGenerator.py
  VERBATIM
)


# Will automatically call the custom command because of the dependency on Interface.cpp
add_library(interflop-interface STATIC "Interface.cpp")
target_include_directories(interflop-interface PUBLIC include)


SET(SRC src/Utils.cpp
        src/Context.cpp 
        src/Interflop.cpp 
)

SET(HEADERS include/Flags.hpp 
            include/Backend.hpp 
            include/OpaqueShadow.hpp
            include/Context.hpp 
)

add_library(interflop-core STATIC ${SRC} ${HEADERS})
target_include_directories(interflop-core PUBLIC include)

add_library(interflop-dummy-core STATIC ${SRC} ${HEADERS})
target_compile_definitions(interflop-dummy-core PUBLIC -DDUMMY_NSAN_INTERFACE)
target_include_directories(interflop-dummy-core PUBLIC include)

add_library(interflop-doubleprec STATIC "src/backends/DoublePrec.cpp"
                                        "include/Context.hpp")
target_include_directories(interflop-doubleprec PUBLIC include)

add_library(interflop-mcasync STATIC "src/backends/MCASync.cpp"
                                     "include/Context.hpp")
target_include_directories(interflop-mcasync PUBLIC include)

set_target_properties(interflop-core interflop-interface interflop-mcasync interflop-doubleprec interflop-dummy-core
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
