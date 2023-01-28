#!/bin/bash

# intel xed
git clone https://github.com/intelxed/xed.git lib/xed
git clone https://github.com/intelxed/mbuild.git lib/mbuild
cd lib/xed
./mfile.py install
cp kits/* ../
cd ../
rm -rf xed mbuild

# Main Project Makefile
cd ../
make