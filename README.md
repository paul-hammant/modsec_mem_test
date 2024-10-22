# ModSecurity Load Test

This project contains a load test for ModSecurity using the `test_modsec_load.c` file.

## Setup

To set up ModSecurity, run the following script:

```bash
./setup_modsecurity.sh
```

This script will clone the ModSecurity repository, build, and install it.

## Prerequisites

- GCC compiler
- ModSecurity library and headers

## Compilation

To compile the load test, run the following command:

```bash
gcc -o test_modsec_load test_modsec_load.c -lmodsecurity
```

## Running the Load Test

After compiling, you can run the load test with:

```bash
./test_modsec_load
```

This will execute the load test, processing a large number of transactions and printing progress periodically.

## Notes

Ensure that the ModSecurity library is properly installed and accessible by the compiler.
