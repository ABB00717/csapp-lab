#!/bin/bash

# This script runs all tests and saves the output to a file.

OUTPUT_FILE="test_results.txt"

# Clear the output file
rm -f $OUTPUT_FILE

# Loop through tests 01 to 16
for i in $(seq -w 1 16)
do
  echo "Running test$i..."
  make test$i >> $OUTPUT_FILE
  echo "" >> $OUTPUT_FILE
done

echo "All tests finished. Results are in $OUTPUT_FILE"