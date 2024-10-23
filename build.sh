#!/bin/bash

# Remove the build directory if it exists
rm -rf build

# Create a new build directory
mkdir build

# Change into the build directory
cd build

# Run cmake to configure the build system
cmake ..

# Build the project using multiple cores
if [ -z "$1" ]
  then
    make -j
  else
    make -j "$1"
fi

# Change back to the parent directory
cd ..


