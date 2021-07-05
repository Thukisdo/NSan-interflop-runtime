CC=../build/bin/clang
CXX=../build/bin/clang++
INCLUDE = include
CCFLAGS = -O3
CXXFLAGS = -O3 -std=c++17 -mavx

all : src/Interface.cpp
	$(CXX) $(CXXFLAGS) -c src/Interface.cpp -o Interface.o -I$(INCLUDE)

Interface.ll : src/Interface.cpp
	$(CXX) $(CXXFLAGS) -S -emit-llvm src/Interface.cpp -o Interface.ll -I$(INCLUDE)


