Pre-installation Instructions
=============================

To install and run libFLASM requires the extraction of the seqan library files
and the maxshiftm project files.
To extract the seqan files, simply extract them from the seqan.zip file to the
root of this project ensuring the header (*.h) files are accessible in ./seqan/*.
To unzip the file on most Linux systems you write `unzip seqan.zip` in the
terminal.
Next, you should extract the maxshiftm.zip archive in much the same way:
`unzip maxshiftm.zip`, so that its files are in ./maxshiftm/*.

Note: We have only tested the program on Linux and cannot guarantee it works
with Microsoft Windows or other operating systems.

Basic Compilation Instructions
==============================

Compilation requires a C++ 11 standard compiler such as the GNU gcc compiler.
An example Makefile to compile the program is available in the example folder.
Simply run `make` to compile the program.
