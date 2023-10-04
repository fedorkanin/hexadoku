#!/bin/bash

# PROG=./bin/main.out
PROG="./$1"
echo "Running tests for $PROG"
TESTS_DIRS=("data/basic" "data/extra")

# Function to clean up temporary files
clean_up() {
  rm -f time.txt test_out.txt
}

# Function to run tests for a given directory
run_tests() {
  local tests_dir=$1
  for IN_FILE in "$tests_dir"/*_in.txt ; do
    REF_FILE="${IN_FILE/_in.txt/_out.txt}"
    { time $PROG < "$IN_FILE" > test_out.txt ; } 2> time.txt
    if ! diff "$REF_FILE" test_out.txt ; then
      echo "Fail: $IN_FILE"
      echo "REF FILE: $REF_FILE"
      clean_up
      exit 1
    else
      echo "OK: $IN_FILE" | tr '\n' ' '
      cat time.txt | tr '\n' ' ' | tr -s ' '
      echo ""
    fi
  done
}

# Run tests for each directory
for tests_dir in "${TESTS_DIRS[@]}"; do
  run_tests "$tests_dir"
  echo ''
done

clean_up

echo ''
