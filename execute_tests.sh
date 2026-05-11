#!/bin/bash
./compile.sh
ctest --test-dir build --output-on-failure
