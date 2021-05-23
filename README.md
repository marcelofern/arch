# ARCH

## What?

`arch` is a binary program that I use to install or update my current arch linux
environment. This repository contains the C code used for generating this
binary

## Dependencies
The only dependency is `base-devel`, so that we have the GNU `make` program
used for compiling the binary.

## Building the package.
```sh
git clone https://github.com/marcelofern/arch.git
cd arch
make
cd ..
rm -rf arch
```

## Usage

1. Installing packages after a fresh arch linux install:
```sh
arch -i
```
2. Updating system after installation (recommended to do this once a week).
```sh
arch -u
```
