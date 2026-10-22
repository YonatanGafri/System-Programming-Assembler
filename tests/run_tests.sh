#!/bin/bash
set -e

echo "Compiling and running data_struct tests..."
gcc -ansi -Wall -g -pedantic -Iheader_files tests/test_data_struct.c object_files/data_struct.o -o tests/test_data_struct
./tests/test_data_struct

echo ""
echo "Compiling and running helper_functions tests..."
gcc -ansi -Wall -g -pedantic -Iheader_files tests/test_helper_functions.c object_files/helper_functions.o object_files/data_struct.o object_files/check_text.o object_files/pre_assembler.o object_files/ass_other.o -o tests/test_helper_functions
./tests/test_helper_functions

echo ""
echo "All tests passed successfully!"
