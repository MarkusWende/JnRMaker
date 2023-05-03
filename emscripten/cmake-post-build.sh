#!/bin/bash

# Copy to server
rm -rf ../../JnRMakerServer/build/www/*
cp build/out/jnrmaker* ../../JnRMakerServer/build/www/
cp ../resources/www/index.html ../../JnRMakerServer/build/www/
cp ../resources/www/favicon.ico ../../JnRMakerServer/build/www/

# Navigate into the output folder then start a simple server and open it.
cd ../../JnRMakerServer/build
./webserver