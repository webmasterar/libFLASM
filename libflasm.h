
#ifndef __LIBFLASM__
#define __LIBFLASM__

#include <seqan/find.h>

using namespace std;
using namespace seqan;

// stores a single result tuple
struct ResultTuple {
    int pos_t;
    int pos_x;
    int error;
};

multiset<ResultTuple,bool(*)(ResultTuple,ResultTuple)> flasm ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error);

#endif

