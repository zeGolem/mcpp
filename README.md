# mcpp

A Minecraft server written in C++ from scratch.

# Currently working

* Basic C++ implementation of Minecraft's protocol
* Simple C++ socket wrapper for TCP server

# How to build/run

## Compatibility

This was tested on the following configurations:

* Linux using GCC/g++ 11.1.0 (My main rig, should always work on this config, also tested with clang here, seems to work.)
* Windows using GCC/g++ 10.2.0 from [MSYS2](https://www.msys2.org/).
  It builds & runs from MSYS2, but requiers MSYS2-specific DLLs to do so. I don't have much experience with Windows C++ 
  dev. but if anyone wants to improve compatibility, please do so.

*If it worked for you on another configuration, please open a PR updating this list, or fixing any bugs you find*.

## Build & Run on your system

1. Create the build directory:

```bash
mkdir build
```

2. Make a normal build:

```bash
make build/mcpp
```

--OR--

2. Make a debug build for use with GDB:

```bash
make build/mcpp.dbg
```

3. Run it:

```bash
cd build
./mcpp
```

--OR--

3. Run it with GDB:

```bash
gdb -ex "cd build" -ex run build/mcpp.dbg # This will setup the directory correctly and run the binary right after starting GDB.
```

# TODO

* Better Makefile (add support for partial builds, and use multiple jobs)
* Full protocol support
* Many code cleanup stuff...
