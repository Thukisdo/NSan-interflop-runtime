
CC = ../../build/bin/clang
CXX = ../../build/bin/clang++
CFLAGS = -g
CXXFLAGS = -g -std=c++17 -mavx


libruntime.a : cpp_dp/runtime.cpp cpp_dp/interface.cpp cpp_dp/runtime.hpp cpp_dp/shadow.hpp
	$(CXX) $(CXXFLAGS) -c -fno-omit-frame-pointer cpp_dp/runtime.cpp
	$(CXX) $(CXXFLAGS) -c -fno-omit-frame-pointer cpp_dp/interface.cpp
	ar rcs libruntime.a runtime.o interface.o

runtime.ll: cpp_dp/runtime.cpp cpp_dp/interface.cpp cpp_dp/runtime.hpp cpp_dp/shadow.hpp
	$(CXX) $(CXXFLAGS) -S -emit-llvm -fno-omit-frame-pointer -c cpp_dp/runtime.cpp

interface.ll: cpp_dp/runtime.cpp cpp_dp/interface.cpp cpp_dp/runtime.hpp cpp_dp/shadow.hpp
	$(CXX) $(CXXFLAGS) -S -emit-llvm -fno-omit-frame-pointer -c cpp_dp/interface.cpp	


clean :
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f *.ll
	rm -f *.s