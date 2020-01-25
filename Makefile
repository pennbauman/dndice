# Penn Bauman
# pennbauman@protonmail.com

CXX=clang++

dndice: dndice.cpp node.h
	$(CXX) dndice.cpp

test: dndice
	./a.out 2d6 + 3
	./a.out +4
