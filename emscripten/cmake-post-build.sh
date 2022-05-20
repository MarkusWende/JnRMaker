#!/bin/bash

# Copy our 'index.html' and 'favicon.ico' into the output folder.
cp templates/index.html build/index.html
cp templates/favicon.ico build/favicon.ico
cp templates/server.py build/server.py
cp templates/key.pem build/key.pem
cp templates/cert.pem build/cert.pem

# Navigate into the output folder then start a simple server and open it.
cd build
/usr/bin/python3 server.py
#python3 -m http.server