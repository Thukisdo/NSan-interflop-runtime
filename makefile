


CC = ../build/bin/clang
CXX = ../build/bin/clang++
RUNTIME = runtime/interface.o  #-Lruntime -lruntime 
CFLAGS = -g
CXXFLAGS = -O3 -g
TEST_PATH = ../llvm-project/compiler-rt/test/nsan
TEST = $(TEST_PATH)/cadna_ex7.cc
RT = libruntime.a

interflop_nsan : runtime 
	$(CXX) $(CXXFLAGS) -g -fsanitize=numerical -fno-omit-frame-pointer -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj main.cpp $(RUNTIME)

native_nsan : 
	$(CXX) $(CXXFLAGS) -fsanitize=numerical -fno-omit-frame-pointer main.cpp

native_example : 
	$(CXX) $(CXXFLAGS) -fsanitize=numerical -fno-omit-frame-pointer $(TEST)

native_ir : 
	$(CXX) $(CXXFLAGS) -S -emit-llvm -fsanitize=numerical -fno-omit-frame-pointer $(TEST)	

interflop_example : runtime 
	$(CXX) $(CXXFLAGS) -fsanitize=numerical -fno-omit-frame-pointer -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj $(TEST) $(RUNTIME)

test : runtime 
	$(CXX) $(CXXFLAGS) -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj -fno-omit-frame-pointer test.cpp $(RUNTIME)

test_native : runtime 
	$(CXX) $(CXXFLAGS) -fsanitize=numerical -fno-omit-frame-pointer test.cpp $(RUNTIME)

test_ir :
	$(CXX) $(CXXFLAGS) -S -emit-llvm -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj -fno-omit-frame-pointer  test.cpp

llvm_link : runtime
	$(CXX) $(CXXFLAGS) -S -emit-llvm -fsanitize=numerical -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj -fno-omit-frame-pointer -o test.ll $(TEST)
	../build/bin/llvm-link test.ll runtime/interface.ll | ../build/bin/llvm-dis -o linked.ll

llvm_build : 
	../build/bin/opt -O3 linked.ll | ../build/bin/llvm-dis -o opt.ll
	$(CXX) -O3 -o linked_opt opt.ll ../build/lib/clang/13.0.0/lib/linux/libclang_rt.nsan-x86_64.a -pthread -ldl


EXAMPLE = cadna_ex1.cc cadna_ex2.cc cadna_ex3.cc cadna_ex4.cc cadna_ex5.cc cadna_ex6.cc cadna_ex7.cc verificarlo_case4.cc

.PHONY: runtime

runtime :
	$(MAKE) -C runtime $(RT)
	#$(MAKE) -C runtime interface.ll


.PHONY: example
example : runtime
	for file in $(EXAMPLE); do \
		$(CXX) -o example/$$file.o $(CXXFLAGS) -fsanitize=numerical -fno-omit-frame-pointer -mllvm --nsan-interflop -mllvm --nsan-shadow-type-mapping=ijj $(TEST_PATH)/$$file $(RUNTIME); \
	done

clean :
	$(MAKE) -C runtime clean
	rm -f *.out *.ll *.txt *.s *.o *.a *.so
	rm -f runtime/*.o
	rm -f example/*.o
	rm -f linked_opt


# $(CXX) -flto -O3 truc.ll ../build/lib/clang/13.0.0/lib/linux/libclang_rt.nsan-x86_64.a -pthread -ldl