#!/bin/bash

# Copy to server
rm -rf ../../JnRMakerServer/build/www/*
cp build/jnrmaker* ../../JnRMakerServer/build/www/
cp build/index.html ../../JnRMakerServer/build/www/
cp build/favicon.ico ../../JnRMakerServer/build/www/

# Navigate into the output folder then start a simple server and open it.
cd ../../JnRMakerServer/build
./webserver