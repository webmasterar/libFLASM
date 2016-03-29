## Example program using libFLASM

## Running

The program is run like so:

`./flasm -m 0 -i input.fas -o positions.out -l 7 -k 1`

The arguments are:

    - m <uint> The method to use. 0 = Edit distance, 1 = Hamming distance.
    - i <file> The input file should be in multiFASTA format with two sequences.
    - o <file> The results will be output to a file as a line seperated list of tuples (p_t, p_x, hed) - (ending position in t, ending position in x, Hamming/edit distance).
    - l <uint> The length of the factor. This must be less than or equal to the length of x.
    - k <uint> The maximum distance permitted in a match between a factor of x and t.

## Example data

The folder ../data/ contains some sample multiFASTA files that we can use. Let's
use 1.fasta and save the results to 1.out in the same directory. A factor length
of 9 will be used for our example and an maximum error distance of 1 too. We will
run the edit distance method first:

`./flasm -m 0 -i ../data/1.fasta -o ../data/1.out -l 9 -k 1`

The output file contains:

```
#(end_pos_t,end_pos_x,error)
(10,33,1)
(15,38,1)
(16,39,1)
(20,43,1)
(46,18,1)
```

All of our results have at least one error and are listed in order of least error, ending position in t and in x. As this example uses simple edit distance, the matching factor in t
can be as long as l + k or as short as l - k with insertions and deletions respectively.

## Compiling

To compile the program, please read ../INSTALL.md.

