# mcpp

A Minecraft server written in C++ from scratch.

# Currently working

* Basic C++ implementation of Minecraft's protocol
* Simple C++ socket wrapper for TCP server

# How to build/run

*(Note: this was only tested on Linux, YMMV)*

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

