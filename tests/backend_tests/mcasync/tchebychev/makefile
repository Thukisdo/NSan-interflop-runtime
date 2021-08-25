
CC = clang
CXX = clang++
SOURCEDIR=../../../..
CFLAGS = -O3 -I$(SOURCEDIR)/include
CXXFLAGS = -O3 -I$(SOURCEDIR)/include -std=c++17
LINK_LIBRARY = -L$(SOURCEDIR)/release/lib -linterflop-mcasync -linterflop-dummy-core -pthread


run: rounding.o plot.py
	./rounding.o
	./plot.py

rounding.o: rounding.cpp	
	$(CXX) $(CXXFLAGS) -o rounding.o rounding.cpp $(LINK_LIBRARY)

re : clean run

clean : 
	rm -f *.dat
	rm -f *.o
	rm -f *.pdf	