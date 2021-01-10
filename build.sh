#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one
#Use: chmod u+x build.sh to compile
cd src
mkdir build
cd build
cmake ../..
make
exit 0
