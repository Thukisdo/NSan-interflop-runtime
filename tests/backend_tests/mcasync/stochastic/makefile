
CC = clang
CXX = clang++
SOURCEDIR=../../../..
CFLAGS = -O3 -I$(SOURCEDIR)/include
CXXFLAGS = -O3 -I$(SOURCEDIR)/include -std=c++17
LINK_LIBRARY = -L$(SOURCEDIR)/release -linterflop

run: test.o plot.py
	./test.o
	./plot.py

test.o: test.cpp	
	$(CXX) $(CXXFLAGS) -o $@ $< $(LINK_LIBRARY)

clean : 
	rm -rf output
	rm	-rf one.o random.o
	rm -rf out.pdf