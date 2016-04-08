# libFLASM

GNU GPLv3 License; Copyright (C) 2016 Lorraine A. K. Ayad, Solon P. Pissis and Ahmad Retha.

The libFLASM library can perform Fixed Length Approximate String Matching under
two distance models: Edit distance and Hamming distance.

Given a text *t* of length *n* and a pattern *x* of length *m*, libFLASM finds
any factor of length *h* <= *m* from *x* in *t* and reports the ending positions
where it finds matches.

The libFLASM library uses the namespace *libflasm* and you should include the
header of the library like so:

```
#include <libflasm.h>

using namespace libflasm;
```

The function signatures of libFLASM are:

```
/**
 * This is the libFLASM edit distance function.
 *
 * @param t The text (haystack) to search in
 * @param n The length of t
 * @param x The pattern which has factors that may be present in t
 * @param m The length of x
 * @param factor_length The length of a factor (needle)
 * @param max_error The maximum distance between the factor and a position in t to report
 * @return The discovered positions are returned in a set that can be iterated over
 */
ResultTupleSet flasm_ed ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error );

/**
 * This is the libFLASM Hamming distance function.
 *
 * @param t The text (haystack) to search in
 * @param n The length of t
 * @param x The pattern which has factors that may be present in t
 * @param m The length of x
 * @param factor_length The length of a factor (needle)
 * @param max_error The maximum distance between the factor and a position in t to report
 * @return The discovered positions are returned in a set that can be iterated over
 */
ResultTupleSet flasm_hd ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error );
```
These methods return a set of tuples and each tuple contains:

```
/**
 * A result tuple contains:
 * 	- pos_t The ending position of the match in t
 * 	- pos_x The ending position of the match in x
 * 	- error The distance between the factor and the match in t
 */
struct ResultTuple {
    unsigned int pos_t;
    unsigned int pos_x;
    unsigned int error;
};
```

The library also provides an iterator to loop through the results, each item of which is a ResultTuple:

```
/* example of how to iterate through a resultset */
ResultTupleSetIterator it;
for ( it = results.begin(); it != results.end(); ++it )
{
    ResultTuple res = *it;
    cout << "(" << res.pos_t << "," << res.pos_x << "," << res.error << ")" << endl;
}
```

To compile the library to create libflasm.so please read INSTALL.md.

To find out more about how to use the library you can look in the ./example folder.

## Citations

The simple edit distance model makes use of Gene Myer's bit-vector algorithm
as implemented by the SeqAn project. The Hamming distance model makes use of
the MaxShiftM implementation of the MaxShift algorithm. If you make use of this
library please cite these references:

    Andreas Döring, et al. "SeqAn an efficient, generic C++ library for sequence analysis." BMC bioinformatics 9.1 (2008): 11.

    Gene Myers. "A fast bit-vector algorithm for approximate string matching based on dynamic programming." J. ACM 46, 3 (May 1999), 395-415.

    Solon Pissis and Ahmad Retha. "Generalised Implementation for Fixed-Length Approximate String Matching under Hamming Distance and Applications." Parallel and Distributed Processing Symposium Workshop (IPDPSW), 2015 IEEE International. IEEE, 2015.
