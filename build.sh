#!/bin/bash

# Compile the test_modsec_load.c with the correct include path for ModSecurity
gcc -o test_modsec_load test_modsec_load.c -I/usr/local/modsecurity/include -L/usr/local/modsecurity/lib -lmodsecurity -Wl,-rpath,/usr/local/modsecurity/lib

echo "Build completed successfully."
