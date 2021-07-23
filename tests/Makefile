
# Compiler path
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -O3
CXXFLAGS = -O3
INSTRUMENTATION_FLAGS = -fno-omit-frame-pointer -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj
LINK_LIBRARY = -L../release/lib  -linterflop-interface -linterflop-doubleprec -linterflop-core

test.o: large_array_fill.cpp
	$(CXX) $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) -o test.o nsan_tests/verificarlo_case4.cc $(LINK_LIBRARY)

clean :
	rm -f *.o
	rm -f *.ll
	rm -f *.out