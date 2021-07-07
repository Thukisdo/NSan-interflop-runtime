
# Compiler path
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -O3
CXXFLAGS = -O3
INSTRUMENTATION_FLAGS = -fno-omit-frame-pointer -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj
LINK_LIBRARY = -L../release -linterflop

large_array_fill: large_array_fill.cpp
	$(CXX) $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) nsan_tests/cadna_ex7.cc $(LINK_LIBRARY)

