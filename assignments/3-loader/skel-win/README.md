# Skeleton for building a PE loader

## Introduction
This project contains a skeleton for building a
[PE](https://en.wikipedia.org/wiki/Portable_Executable) binary
on-demand loader in Windows. This skeleton can be loaded in
Microsoft Visual Studio C++ and develop the solution from there.
The loader will provide two methods, defined in the `loader.h`
header:
* `int so_init_loader(void);` - initializes the on-demand loader
* `int so_execute(char *path, char *argv[]);` - executes the binary located in
`path` with the required `argv` arguments.

## Content
The project contains of three components, each of them in its own
directory:
* `loader` - a dynamic library that can be used to run PE binaries. It
consists of the following files:
  * `exec_parser.c` - Implements a PE binary parser.
  * `exec_parser.h` - The header exposed by the PE parser.
  * `loader.h` - The interface of the loader, described in the
  [Introduction](#introduction) section.
  * `loader.c` - This is where the loader should be implemented.
  * `debug.h` - header for the `dprintf` function that can be used for logging
  and debugging.
* `exec` - a program that uses the `so_loader.dll` library to run a PE
binary received as argument.
* `test_prog` - an PE binary used to test the loader implementation.

There project also contains 2 makefiles:
* `Makefile` - builds the `so_loader.dll` library from the `loader`
directory
* `Makefile.example` - builds the `so_exec.exe` and `so_test_prog.exe` binaries
from the `exec` and `test_prog` directories that can be used to test the loader.

The project also contains files that can be used to import the skeleton
as a Microsoft Visual Studio C++ solution. After you open the solution, make
sure you are setting the `exec` project as StartUp Project.

## Usage Build the loader:
```
nmake
```

This should generate the `so_loader.dll` library. Next, build the example:

```
nmake /f Makefile.example
```

This should generate the `so_exec.exe` and `so_test_prog.exe` used for the test:

```
.\so_exec.exe so_test_prog.exe
```

**NOTE:** the skeleton does not have the loader implemented, thus when running
the command above, your program will crash!

## Notes
This skeleton is provided by the Operating System team from the University
Politehnica of Bucharest to their students to help them complete their
Executable Loader assignment.
