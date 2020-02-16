#!/bin/bash
cd ./build && find . ! -name 'CMakeLists.txt' -type f -exec rm -f {} + && rm -r CMakeFiles
