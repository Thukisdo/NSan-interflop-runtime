


# FIXME: this file shouldn't exist, and is only here for testing purposes
# Should be removed in the future
# and nsan tests should be handled otherwise

# Compiler path
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -O3
CXXFLAGS = -O3
INSTRUMENTATION_FLAGS = -mavx -fsanitize=numerical -mllvm -nsan-interflop -mllvm -nsan-shadowscale=4
LINK_LIBRARY = -lm -lgfortran -ldl -pthread -L../debug/lib -linterflop-interface -linterflop-mcasync -linterflop-core #/home/chps/Bureau/work/nsan/build/lib/clang/13.0.0/lib/linux/libclang_rt.nsan-x86_64.a


TEST=main.cpp

.PHONY: test.o
test.o: 
	$(CXX) $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) -o test.o $(TEST) $(LINK_LIBRARY)

IR : 
	$(CC) -S -emit-llvm $(CXXFLAGS) $(INSTRUMENTATION_FLAGS) -c -o test.ll $(TEST) $(LINK_LIBRARY)

run:
	./test

clean :
	rm -f test.o
	rm -f *.ll
	rm -f *.out