# Project README

## Overview
The project is a C/C++ application designed to demonstrate the use of a neural network for recognizing handwritten digits. It includes features such as training a simple neural network, visualizing the network's performance, and running on various platforms.

## Features
- Neural Network Training and Testing for Handwritten Digits
- Visualization of Training Performance using Plotting Library
- Cross-platform support: Linux, Windows, Wine (Windows cross-compilation), and WebAssembly

## Project Structure
- `build/`: Compiled executable files produced by the build process.
- `libs/`: Placeholder for library source files if needed.
- `src/`: Source code directory containing:
  - `Main.c`: Entry point of the application.
  - Additional header files used in the project.

### Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- Libraries required for GUI and image processing: X11 for Linux, user32, gdi32, winmm for Windows/Wine

## Build & Run
The project uses different Makefiles for building on various platforms.

### Linux
To build on Linux:
```sh
cd <Project>
make -f Makefile.linux all
```

To run the application:
```sh
make -f Makefile.linux exe
```

### Windows
To build on Windows (assuming a Unix-like environment like WSL or Cygwin):
```sh
cd <Project>
make -f Makefile.windows all
```

To run the application:
```sh
make -f Makefile.windows exe
```

### Wine
For building and running on Wine, which is used for Windows cross-compilation:
```sh
cd <Project>
make -f Makefile.wine all
```

To run the application:
```sh
make -f Makefile.wine exe
```

### WebAssembly
For compiling to WebAssembly:
```sh
cd <Project>
make -f Makefile.web all
```

To serve and run the WebAssembly application:
```sh
make -f Makefile.web exe
```

This setup ensures that the project can be built and executed on different platforms, providing a versatile environment for development and testing.