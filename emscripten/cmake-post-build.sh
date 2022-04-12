#!/bin/bash

# Copy our 'index.html' and 'favicon.ico' into the output folder.
cp templates/index.html build/index.html
cp templates/favicon.ico build/favicon.ico

# Navigate into the output folder then start a simple server and open it.
cd build
python3 -m http.server