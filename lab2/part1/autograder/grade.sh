#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <filename>"
else
    cd ref
    gcc *.c -o $1
    count=$(find . -name "*.out" | wc -l)
    if [[ $count -ne 0 ]]; then
        # echo "rm *.out"
        rm *.out
    # else
    #     echo "no out file"
    fi

    for i in *.in; do
        # echo "$1 < $i > $i.out"
        ./$1 < $i > $i.out
    done

fi

# Delete temporary files

# Compile the reference program

# Generate reference output files

# Now mark submissions

#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#

# Iterate over every submission directory
    # Compile C code
    # Print compile error message to output file
    # Generate output from C code using *.in files in ref
    # Compare with reference output files  and award 1 mark if they are identical
# print score for student
# print total files marked.
    
