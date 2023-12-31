#!/usr/bin/bash #

# testing.sh- This file will test whether coolness.c works
#
# input: None
# output: Whether coolness.c works
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

#No argument test

#Put no argument output into output text file and print the text file
./coolness > output.txt
cat output.txt
# Check number of lines
num_lines=$(wc -l < output.txt)
#Awk command to exit with one if a line doesn't have 3 columns
awk 'NF > 0 && NF != 3 { exit 1 }' output.txt
awk_status=$?
#If lines is equal to 24 and exit status is equal to 0
if [[ $awk_status -eq 0 && $num_lines -eq 24 ]]; then
	#If they do then no argument output is successful
	echo "No argument output: Columns and Lines Match- Success!"
else
	#Else it fails	
	echo "No argument output: Columns or Lines Don't Match- Fail"
fi

#One argument test

#Put one argument output into output text file and print the text file
./coolness 32 > output.txt
cat output.txt
#Check number of lines
num_lines=$(wc -l < output.txt)
#Check number of columns
awk '{if (NF != 3) exit 1}' output.txt

awk_status=$?
#If lines is equal to 4 and exit status is equal to 0
if [[ $awk_status -eq 0 && $num_lines -eq 4 ]]; then

        #If they do then one argument output is successful
        echo "One argument output: Columns and Lines Match- Success!"
else
        #Else it fails  
        echo "One argument output: Columns or Lines Don't Match- Fail"
fi

#Two argument test

#Put one argument output into output text file and print the text file
./coolness 32.5 10 > output.txt
cat output.txt
#Check number of lines
num_lines=$(wc -l < output.txt)
#Check number of columns
awk '{if (NF != 3) exit 1}' output.txt

awk_status=$?
#If lines is equal to 2 and exit status is equal to 0
if [[ $awk_status -eq 0 && $num_lines -eq 2 ]]; then
        #If they do then two argument output is successful
        echo "Two argument output: Columns and Lines Match- Success!"
else
        #Else it fails
        echo "Two argument output: Columns or Lines Don't Match- Fail"
fi


#Remove files
make clean
rm output.txt



