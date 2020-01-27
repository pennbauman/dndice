# Penn Bauman
# pennbauman@protonmail.com

CXX=clang++

dndice: dndice.cpp
	$(CXX) dndice.cpp

test: dndice
	./a.out 2d6 + 3
	@echo ""
	./a.out +4

clean:
	@rm -f *.o *.out dndice
