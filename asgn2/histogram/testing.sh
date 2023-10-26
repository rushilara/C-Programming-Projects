#!/usr/bin/bash #

# testing.sh- This file will test whether histogram.c works
#
# input: None
# output: Whether histogram.c works
#
# usage: bash testing.sh
#
# Rushil Aramandla, ID# 1940008, raramand@ucsc.edu

#Compile the file
make

#Check if compilation was successful
if [[ $? -ne 0 ]]; then
    echo "Compilation failed"
    exit 1
fi

make clean
