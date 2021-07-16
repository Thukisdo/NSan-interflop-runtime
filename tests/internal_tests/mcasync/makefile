
CC = clang
CXX = clang++
CFLAGS = -O3 -I../../../include
CXXFLAGS = -O3 -I../../../include -std=c++17
LINK_LIBRARY = -L../../../release -linterflop 


run: rounding.o plot.py
	./rounding.o
	./plot.py out.dat 53

rounding.o: rounding.cpp	
	$(CXX) $(CXXFLAGS) -o rounding.o rounding.cpp $(LINK_LIBRARY)

clean : 
	rm -f out.dat
	rm -f rounding.o
	rm -f out-53.pdf	