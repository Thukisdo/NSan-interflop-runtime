
CC = clang
CXX = clang++
CFLAGS = -O3
CXXFLAGS = -O3
LINK_LIBRARY = -L../../../release -linterflop


run: rounding.o plot.py
	./rounding.o
	./plot.py out.dat 53

rounding.o: rounding.cpp	
	$(CXX) $(CXXFLAGS) -o rounding.o rounding.cpp $(LINK_LIBRARY)

clean : 
	rm -f out.dat
	rm -f rounding.o
	rm out-53.pdf	