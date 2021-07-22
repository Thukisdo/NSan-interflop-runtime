
CC = clang
CXX = clang++
SOURCEDIR=../../../..
CFLAGS = -O3 -I$(SOURCEDIR)/include
CXXFLAGS = -O3 -I$(SOURCEDIR)/include -std=c++17
LINK_LIBRARY = -L$(SOURCEDIR)/release -linterflop


# Select the float type to use for the test
REALTYPE = FLOAT


run: rounding.o plot.py
	./rounding.o
	./plot.py out.dat 53

rounding.o: rounding.cpp	
	$(CXX) $(CXXFLAGS) -D $(REALTYPE) -o rounding.o rounding.cpp $(LINK_LIBRARY)

clean : 
	rm -f out.dat
	rm -f rounding.o
	rm -f out-53.pdf	