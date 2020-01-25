# Penn Bauman
# pennbauman@protonmail.com

CXX=clang++

make: dndice.cpp node.h
	$(CXX) dndice.cpp

test: make
	./a.out 2d6 + 3

final: dndice.cpp node.h
	$(CXX) -O2 dndice.cpp -o dndice
