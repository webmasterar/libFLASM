Pre-installation Instructions
=============================

To install and run libFLASM requires the extraction of the seqan library files.
Simply extract them from the seqan.zip file to the root of this project ensuring
the header (*.h) files are accessible in ./seqan/*. To unzip the file on most
Linux systems you write `unzip seqan.zip` in the terminal.

Basic Compilation Instructions
==============================

Note: We have only tested the program on Linux and cannot guarantee it works
with Microsoft Windows or other operating systems even with modifications.

Compilation requires a C++ 11 standard compiler such as the GNU gcc compiler.
Simply run `make` to compile the program into a library. You may now use the
created libflasm.so library in your projects. Please look in the ./example/
folder to see how you might use this library with your own programs.
