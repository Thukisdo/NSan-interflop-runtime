
CC = clang
CXX = clang++
SOURCEDIR=../../../..
CFLAGS = -O3 -I$(SOURCEDIR)/include
CXXFLAGS = -O3 -I$(SOURCEDIR)/include -std=c++17
LINK_LIBRARY = -L$(SOURCEDIR)/release -linterflop

run: one.o plot.py
	./one.o
	./plot.py

one.o: one.cpp	
	$(CXX) $(CXXFLAGS) -o one.o one.cpp $(LINK_LIBRARY)

clean : 
	rm -f out.dat
	rm -f one.o
	rm -f out-53.pdf	