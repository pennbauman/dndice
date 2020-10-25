#!/bin/sh
# Penn Bauman
# pennbauman@protonmail.com

echo -n "Compiling..."
g++ -O2 dndice.cpp -o dndice
echo "Done"

sudo mv dndice /bin/
echo "Installed"
