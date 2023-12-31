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

#First Test

#Put first test output into output.txt
#I will use printf for the input because of how it handles white space differently than the echo command
printf "3 -4 5 1 7 0\n8 0 15 12 3 5\n3 3 3 3 3\n" | ./histogram > output.txt
#Print output text file
cat output.txt
#Create variable count that is equal to the # of asterisks in the text file
count=$(grep -o '*' output.txt | wc -l)
#If loop to check that count is equal to the correct # of asterisks: 16. If not test fails
if [ "$count" -eq 16 ]; then
	echo "First test passes based off # of asterisks- make sure to still eye check"
else
	echo "First test fails due to incorrect # of asterisks"
fi

#Second Test 

#Generating output
printf "3 -4 5 1 7 0\n8 0 15 12 3 5\n18 19 20 30 7 12\n50 34\n32\n19\n44\n" | ./histogram > output.txt
#Print output text file
cat output.txt
#Create variable count that is equal to the # of asterisks in the text file
count=$(grep -o '*' output.txt | wc -l)
#If loop to check that count is equal to the correct # of asterisks: 22. If not test fails
if [ "$count" -eq 22 ]; then
        echo "First resizing test passes based off # of asterisks- make sure to still eye check"
else
        echo "First resizing test fails due to incorrect # of asterisks"
fi

#Third Test

#Generating output
printf "150\n" | ./histogram > output.txt
#Print output text file
cat output.txt
#Create variable count that is equal to the # of asterisks in the text file
count=$(grep -o '*' output.txt | wc -l)
#If loop to check that count is equal to the correct # of asterisks: 1. If not test fails
if [ "$count" -eq 1 ]; then
        echo "Second resizing test passes based off # of asterisks- make sure to still eye check"
else
        echo "Second resizing test fails due to incorrect # of asterisks"
fi




#Generating output
echo {1..16} 150 | ./histogram > output.txt
#Print output text file
cat output.txt
#Create variable count that is equal to the # of asterisks in the text file
count=$(grep -o '*' output.txt | wc -l)
#If loop to check that count is equal to the correct # of asterisks: 17. If not test fails
if [ "$count" -eq 17 ]; then
        echo "Last test passes based off # of asterisks- make sure to still eye check"
else
        echo "Last resizing test fails due to incorrect # of asterisks"
fi



#Remove files
rm output.txt
make clean
