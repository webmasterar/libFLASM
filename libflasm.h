
#ifndef __LIBFLASM__
#define __LIBFLASM__

#include <seqan/find.h>

using namespace std;
using namespace seqan;

// stores a single result tuple and provides comparator (operator) function for sorting results
struct ResultTuple {
    bool operator() (const ResultTuple& a, const ResultTuple& b) const {return ( a.error < b.error ) || ( a.error == b.error && a.pos_t < b.pos_t ) || ( a.error == b.error && a.pos_t == b.pos_t && a.pos_x < b.pos_x );};
    unsigned int pos_t;
    unsigned int pos_x;
    unsigned int error;
};

// iterator
typedef std::multiset<ResultTuple,ResultTuple>::iterator ResultTupleSetIterator; 

// FLASM Edit distance
std::multiset<ResultTuple,ResultTuple> flasm_ed ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error);

// FLASM Hamming distance
std::multiset<ResultTuple,ResultTuple> flasm_hd ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error);

#endif
