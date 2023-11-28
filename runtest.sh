#!/bin/sh

# runtest.sh - Script to run all unit tests
# @ Author: Meng Chen
# @ Date: 11/28/2023
echo "Running all unit tests..."

# Running each test script
echo "1. Running Encoder Tests"
./bin/test_encoder_unit.sh

echo "2. Running Scanner Tests"
./bin/test_scanner_unit.sh

echo "3. Running Parser Tests"
./bin/test_parser_unit.sh

echo "4. Running Printer Tests"
./bin/test_printer_unit.sh

echo "5. Running Resolver Tests"
./bin/test_resolver_unit.sh

echo "6. Running Typechecker Tests"
./bin/test_typechecker_unit.sh

echo "All unit tests completed."
