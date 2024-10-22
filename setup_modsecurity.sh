#!/bin/bash

# Clone the ModSecurity repository
git clone --depth 1 git@github.com:owasp-modsecurity/ModSecurity.git

# Navigate to the ModSecurity directory
cd ModSecurity

git submodule init
git submodule update

# Build ModSecurity
./build.sh
./configure
make
sudo make install

# Ensure headers are installed
sudo cp -r headers/modsecurity /usr/local/include/

# Return to the original directory
cd ..

echo "ModSecurity has been successfully built and installed."
