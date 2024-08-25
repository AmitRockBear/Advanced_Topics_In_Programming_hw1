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
make -j

# Change back to the parent directory
cd ..


