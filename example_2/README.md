## Example program using libFLASM

## Running

The program is run like so:

`./dbflasm -m 0 -i input.fas -o positions.out -q 2 -k 1`

The arguments are:

  -m, --model                 <int>     The distance model. 0 = Edit distance, 1 = Hamming distance.
  -i, --input-file            <file>    (Multi)FASTA input filename containing text T and pattern X.
  -o, --output-file           <file>    Output filename for the positions.
  -q, --q-gram-length         <int>     The length of the q-grams.
  -k, --max-error             <int>     The maximum error (edit/Hamming distance) permitted in a match.


## Examples

The folder ../data/ contains some sample multiFASTA files that we can use. Let's
use 1.fasta and save the results to 1.out in the same directory. A q-gram length
of 2 will be used for our example and an maximum error distance of 1. We will
run the edit distance method first:

`./dbflasm -m 0 -i ../data/1.fasta -o ../data/1.out -q 2 -k 1`

The output file contains 16 results:

	#(q_gram,error)
	(AA,0)
	(AC,1)
	(AG,0)
	(AT,1)
	(CA,0)
	(CC,0)
	(CG,1)
	(CT,0)
	(GA,0)
	(GC,0)
	(GG,1)
	(GT,0)
	(TA,0)
	(TC,0)
	(TG,0)
	(TT,0)	


Then we can try doing the search using the Hamming distance method:

`./dbflasm -m 1 -i ../data/1.fasta -o ../data/1.out -q 2 -k 1`

	#(q_gram,error)
	(AA,0)
	(AC,1)
	(AG,0)
	(AT,1)
	(CA,0)
	(CC,0)
	(CG,1)
	(CT,0)
	(GA,0)
	(GC,0)
	(GG,1)
	(GT,0)
	(TA,0)
	(TC,0)
	(TG,0)
	(TT,0)	


Both sets of results contain 16 q-grams, due to the length of the q-gram 
input. A De Bruijn sequence X is created using the alphabet from sequence 2
from the pair of input sequences. Fixed length approximate string matching
is used for each q-gram q in X and text T. The results output show the minimum
errors found between each q-gram and the text T.

## Compiling

To compile the program you need to create the library libflasm.so file first.
Please read ../INSTALL.md to do this, then return to this directory and simply
run `make` on the command line to build this example program.
