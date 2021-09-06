


# FIXME: this file shouldn't exist, and is only here for testing purposes
# Should be removed in the future
# and nsan tests should be handled otherwise

# Compiler path
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -O3
CXXFLAGS = -O3
INSTRUMENTATION_FLAGS = -fno-omit-frame-pointer -fsanitize=numerical -mllvm -nsan-interflop -mllvm -nsan-shadowscale=2
LINK_LIBRARY = -L../release/lib -linterflop-interface -linterflop-doubleprec -linterflop-core
TEST=nsan_tests/verificarlo_case4.cc

.PHONY: test.o
test.o: 
	$(CXX) $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) -o test.o $(TEST) $(LINK_LIBRARY)

clean :
	rm -f test.o
	rm -f *.ll
	rm -f *.out