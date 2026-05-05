#!/bin/bash

chmod +x compile.sh
chmod +x execute_benchmarks.sh
chmod +x execute_tests.sh
chmod +x plot.sh

cmake -S . -B build
./compile.sh
