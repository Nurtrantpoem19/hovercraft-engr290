
# Overview

This guide explains how to build the project and flash it to an AVR board.

## 1. Build the project

From the repository root, configure the build with the AVR toolchain:

```sh
cd hovercraft-engr290
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/avr.cmake
```

Then build the project:

```sh
cmake --build build
```
For every subsequent build (e.g. if you have updated code or added functionality), you can just run 
```
make
```
inside the build folder to recompile executable file.

## 2. Check the serial port

List available USB serial devices:

```sh
ls /dev/ttyUSB* /dev/ttyACM*
```

Open `build/compilation.sh` and update the serial-port setting to match your board. For example, the port may be `/dev/ttyUSB0` or `/dev/ttyACM0`.

## 3. Convert and flash the executable

From the `build` directory, run the compilation script with the generated ELF file:

```sh
cd build
./compilation.sh <project-name>.elf
```

The script converts the ELF file to a HEX file and flashes it to the board connected to the configured serial port.

Replace `<project-name>.elf` with the actual ELF filename. Make sure the board is connected and the serial port is correct before running the script.
```
