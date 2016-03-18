libFLASM
========

GNU GPLv3 License; Copyright (C) 2016 Lorraine A. K. Ayad, Solon P. Pissis and Ahmad Retha.

The application libFLASM performs Fixed Length Approximate String Matching under
the simple edit distance model and makes use of Myer's bit-parallel algorithm
as implemented by the Seqan project.

Given a text t of length n and a pattern x of length m, libFLASM finds any
factor of length h <= m from x in t and report the ending positions where it
finds a match.

The program is run like so:

`./flasm -i input.fas -o positions.out -l 7 -k 1`

The arguments are:

    - i <file> The input file should be in multiFASTA format with two sequences.
    - o <file> The results will be output to a file as a new line seperated list of (p, d) - (ending position in t, edit distance).
    - l <uint> The length of the factor. This must be less than or equal to the length of x.
    - k <uint> The maximum distance permitted in a match between a factor of x and t.

To compile the program, please read INSTALL.md.

To find out how to use the program you can look at the examples in ./examples

