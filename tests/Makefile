
# Compiler path
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -g
CXXFLAGS = -g
INSTRUMENTATION_FLAGS = -fno-omit-frame-pointer -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj
LINK_LIBRARY = -L../debug -linterflop

large_array_fill: large_array_fill.cpp
	$(CXX) $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) nsan_tests/verificarlo_case4.cc $(LINK_LIBRARY)
