#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <filename>"
else
    cd ref

    # Delete temporary files
    count=$(find . -name "*.out" | wc -l)
    if [[ $count -ne 0 ]]; then
        rm *.out
    fi

    # Compile reference program
    gcc *.c -o $1

    # Generate reference output files
    for i in *.in; do
        ./$1 < $i > $i.out
    done

    # max score
    max=$(find . -name "*.out" -type f | wc -l | sed 's/ //g')

    # touch results.out
    echo -e "Test date and time: $(date +%A), $(date +"%d %B %Y"), $(date +%T)\n" > results.out

    #count number of dir processed
    let num=0

    # Mark submissions
    for dir in ../subs/*; do
        let num++
        FILE=$(basename $dir)
        # echo "$FILE"
        gcc $dir/*.c -o $FILE-res
        if [[ $? -ne 0 ]]; then
            # echo "compilation error"
            echo "Directory $FILE has a compilation error." >> results.out
        fi
        
        let score=0

        for i in *.in; do
            if [[ -f ./$FILE-res ]]; then
                ./$FILE-res < $i > $FILE-$i.out

                if cmp -s -- "$i.out" "$FILE-$i.out"; then
                # if diff "$i.out" "$FILE-$i.out"; then
                    # echo "$i.out and $dir-$i.out have identical contents"
                    let score++
                # else
                    #: # the differences between the files have been listed
                    # echo "files are diff"
                fi
            fi
            
        done
        echo "Directory $FILE score $score / $max" >> results.out
    done

    echo -e "\nProcessed $num files." >> results.out

fi

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
    
