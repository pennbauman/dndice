#!/bin/sh
# Penn Bauman
# pennbauman@protonmail.com

if clang++ --version &> /dev/null; then
	echo "Making (clang++)"
	clang++ -O2 dndice.cpp -o dndice
else
	echo "Making (g++)"
	g++ -O2 dndice.cpp -o dndice
fi

sudo mv dndice /bin/
echo "Installed"
