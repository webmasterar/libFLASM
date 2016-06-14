## Example program using libFLASM

## Running

The program is run like so:

`./flasm -m 0 -i input.fas -o positions.out -l 7 -k 2 -r`

The arguments are:

    - m <uint> The method to use. 0 = Edit distance, 1 = Hamming distance.
    - i <file> The input file should be in multiFASTA format with two sequences.
    - o <file> The results will be output to a file as a line seperated list of tuples (p_t, p_x, hed) - (ending position in t, ending position in x, Hamming/edit distance).
    - l <uint> The length of the factor. This must be less than or equal to the length of x.
    - k <uint> The maximum distance permitted in a match between a factor of x and t.
    - r <void> Optional. If present, all matches are returned, otherwise only the first best match is returned.

## Examples

The folder ../data/ contains some sample multiFASTA files that we can use. Let's
use 1.fasta and save the results to 1.out in the same directory. A factor length
of 9 will be used for our example and an maximum error distance of 1 too. We will
run the edit distance method first:

`./flasm -m 0 -i ../data/1.fasta -o ../data/1.out -l 9 -k 1 -r`

The output file contains 15 matches:

    #(end_pos_t,end_pos_x,error)
    (10,33,0)
    (8,31,1)
    ...
    (20,43,1)
    (46,18,1)

All of our results have one error except the first one which is a perfect match.
Note that the results are listed in order of least error, earliest ending
position in *t* and then in *x*. As this example uses simple edit distance, the
matching factor in *t* can be as long as *l + k* or as short as *l - k* with
insertions and deletions, respectively. If we look at the first and last results
we see how they match up:

    (10,33,0)
    t: GT _GTCTCCCCA_ GAGTCAGAGAGCTTTAGAGTCTAAAGTGCTAAGTAAAAAG
    x: AGTGTAATGCGCTATGTAATAAGGC _GTCTCCCCA_ AAGTCAAAGAACTCTAA

    (46,18,1) T/A mismatch
    t: GTGTCTCCCCAGAGTCAGAGAGCTTTAGAGTCTAAAGT _GCTAAGTAA_ AAAG
    x: AGTGTAATGC _GCTATGTAA_ TAAGGCGTCTCCCCAAAGTCAAAGAACTCTAA

Then we can try doing the search using the Hamming distance method:

`./flasm -m 1 -i ../data/1.fasta -o ../data/1.out -l 9 -k 1 -r`

The Hamming distance method only recognises substitutions so the factors it
discovers will always be *l* in length. Eleven matches are printed in the output
file and most match the ones found using the edit distance method. The edit
distance method allows for results with insertions and deletions so that is why
it finds more matches.

## Compiling

To compile the program you need to create the library libflasm.so file first.
Please read ../INSTALL.md to do this, then return to this directory and simply
run `make` on the command line to build this example program.
