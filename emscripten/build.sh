#!/bin/bash

# Alias the 'pushd' command and have it send its output to the abyss ...
pushd() {
	command pushd "$@" > /dev/stdout
}

# Alias the 'popd' command and have it send its output to the abyss ...
popd() {
	command popd "$@" > /dev/stdout
}

# Make sure we have a 'build' folder.
if [ ! -d "build" ]; then
    mkdir build
fi

# Remove the 'out' folder if it exists.
if [ -d "build" ]; then
    rm -rf out
fi

# In order to find the Emscripten build tools, we need to configure some environment variables so they are available during the build. The required environment variables are initialized by sourcing the 'emsdk_env.sh' that ships with the Emscripten SDK.
pushd ${HOME}/Programme/emsdk
    echo "Configuring Emscripten environment variables"
    . ./emsdk_env.sh
popd

# Hop into the 'build' folder to start our CMake build.
pushd build
    # Because we sourced the Emscripten environment variables, we can use the 'EMSCRIPTEN' var to know where the current SDK can be found, which we need so we can locate the 'Emscripten.cmake' toolchain file.
    EMSCRIPTEN_CMAKE_PATH=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

    # We ask CMake to configure itself against the parent folder, but unlike our other platform targets, we will tell CMake to use the Emscripten CMake toolchain which knows how to perform Emscripten builds.
    echo "Emscripten CMake path: ${EMSCRIPTEN_CMAKE_PATH}"
    cmake -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN_CMAKE_PATH} ..
    #cmake ..

    # Start the actual build.
    echo "Building project ..."
    make
popd
