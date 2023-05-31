#!/usr/bin/python3
from http.server import HTTPServer, BaseHTTPRequestHandler
import os
import ssl

PORT = 8000
IP = "0.0.0.0"
ROOT = os.getcwd()
fileMap = {} # name -> path

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/":
            self.path = "/index.html"
        self.path = self.path[1:]
        #print("GET {}".format(self.path))
        servePath = ""
        directPath = os.path.join(ROOT, self.path)
        #print(directPath)
        if os.path.isfile(directPath):
            #print("Direct serving {} from {}".format(self.path, directPath))
            servePath = directPath
        else:
            head, tail = os.path.split(self.path)
            if tail in fileMap:
                mappedPath = fileMap[tail]
                #print("Mapped serving {} from {}".format(self.path, mappedPath))
                servePath = mappedPath
        if len(servePath) > 0:
            self.send_response(200, "ok")
            self.send_header("Cross-Origin-Opener-Policy", "same-origin")
            self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
            if servePath.endswith(".wasm"):
                self.send_header("Content-Type", "application/wasm")
            elif servePath.endswith(".js"):
                self.send_header("Content-Type", "application/x-javascript")
            self.end_headers()
            with open(servePath, 'rb') as filehandle:  
                self.wfile.write(filehandle.read())
        else:
            self.send_response(404)
            self.end_headers()

print("Buildig file map for {}".format(ROOT))
for root, _ , files in os.walk(ROOT):
    for fileName in files:
        fileMap[fileName] = os.path.join(root, fileName)

print("Starting server https://{}:{}/".format(IP, PORT))
WebServer = SimpleHTTPRequestHandler
WebServer.extensions_map={
        '.manifest': 'text/cache-manifest',
        '.html': 'text/html',
        '.png': 'image/png',
        '.jpg': 'image/jpg',
        '.svg':        'image/svg+xml',
        '.css':        'text/css',
        '.js':        'application/x-javascript',
        '.wasm': 'application/wasm',
        '.data': 'application/data',
        '.json': 'application/json',
        '.xml': 'application/xml',
        '': 'application/octet-stream', # Default
    }
httpd = HTTPServer((IP, PORT), WebServer)
httpd.socket = ssl.wrap_socket (httpd.socket, 
        keyfile="key.pem", 
        certfile='cert.pem', server_side=True)
httpd.serve_forever()
