# ARCH

## What?

`arch` is a binary program that I use to install or update my current Arch Linux
environment (as automatically as possible). This repository contains the C
code used for generating the binary.

## Why?

Over the years I have changed machines multiple times. I have coded on macbooks,
Asus laptops, Thinkpads, customised desktop machines, etc. One recurrent problem
is that every time I switched machines, I needed to re-install all the libraries
and dependencies that I needed in order to carry on with my work. This takes
time and is prone to human errors if done manually. I would often miss libraries
or important tools which would lead to stressful amounts of time figuring
out why my environment was broken.

Now I have been using Arch Linux which is the ideal distribution for me given
that a fresh installation only gives me the bare minimum: A decent package manager
(`pacman`), and `systemd` (which I don't mind at all). This alleviates the
headache of having a broken system due to the dependencies within the system
changing over versions.

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
```
Usage: arch [OPTION...] 

  -d, --dotfiles             Install or sync dotfiles.
  -i, --install              Install environment dependencies from scratch.
  -u, --update               Update environment dependencies.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

