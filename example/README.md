## Example program using libFLASM

## Running

The program is run like so:

`./flasm -i input.fas -o positions.out -l 7 -k 1`

The arguments are:

    - m <uint> The method to use. 0 = Edit distance, 1 = Hamming distance.
    - i <file> The input file should be in multiFASTA format with two sequences.
    - o <file> The results will be output to a file as a line seperated list of tuples (p_t, p_x, hed) - (ending position in t, ending position in x, Hamming/edit distance).
    - l <uint> The length of the factor. This must be less than or equal to the length of x.
    - k <uint> The maximum distance permitted in a match between a factor of x and t.

## Example data

@@todo

## Compiling

To compile the program, please read ../INSTALL.md.

