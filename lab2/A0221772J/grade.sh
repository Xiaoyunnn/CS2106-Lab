#!/bin/bash

# Check if we have enough arguments
if ([[ $# -ne 1 ]]); then
  echo "Usage: $0  <filename>"
  exit
fi

# Delete temporary files
for i in ./ref/*.in.out; do
  rm $i
done

# Compile reference output files
gcc ./ref/*.c  -o $1

# Generate reference output files
for i in ./ref/*.in; do
  ./$1 < $i > "$i.out"
done

# Process time and date
echo -e "Test date and time: $(date +%A), $(date +"%d %B %Y"), $(date +%T)\n" > results.out

# Iterate over every submission
fileNum=0
for i in ./subs/*; do
  studentId=$(basename $i)
  let fileNum=fileNum+1

  error=0

  # Compile C code
  gcc $i/*.c  -o studentFiles

  # Print compile error message to output file
  if [[ $? -ne 0 ]]; then
    echo -e "Directory $studentId has a compile error." >> results.out
    let error=1
  fi

  # Generate output from C code using *.in files in ref
  count=0;
  score=0;
  for k in ./ref/*.in; do
    let count=count+1
    if [[ $error -eq 1 ]]; then
      continue;
    fi
    outputName=$i
    outputName+="/$studentId"
    outputName+="$count.out"

    ./studentFiles < $k > $outputName
    # Now mark submissions
    if cmp --silent $outputName "$k.out"; then
      let score=score+1
    fi
  done
  
  echo -e "Directory $studentId score $score / $count" >> results.out
  
  studentFiles=""
done

echo -e "\nProcessed $fileNum files." >> results.out

