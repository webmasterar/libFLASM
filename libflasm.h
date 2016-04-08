/**
    libFLASM
    Copyright (C) 2016 Lorraine A. K. Ayad, Solon P. Pissis and Ahmad Retha

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef __LIBFLASM__
#define __LIBFLASM__

#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <limits.h>

#include "seqan/find.h"

using namespace std;
using namespace seqan;

namespace libflasm
{

    /**
    * WORD, WORD_SIZE and delta function definitions
    */
    typedef unsigned long int WORD;
    #define BYTE_SIZE 8
    #define WORD_SIZE sizeof ( WORD ) * BYTE_SIZE
    #define delta(a,b) ( ( WORD ) ((a) != (b)) )

    /**
    * A structure to hold number of WORDs and bits needed to store the errors of a
    * factor. Also contains yWord mask to be used to clear the left-most bits on
    * the most significant WORD in the errors array
    */
    struct Limit
    {
	unsigned int words;
	unsigned int h;
	WORD yWord;
    };

    // stores a single result tuple and provides comparator (operator) function for sorting results
    struct ResultTuple {
	bool operator() (const ResultTuple& a, const ResultTuple& b) const {return ( a.error < b.error ) || ( a.error == b.error && a.pos_t < b.pos_t ) || ( a.error == b.error && a.pos_t == b.pos_t && a.pos_x < b.pos_x );};
	unsigned int pos_t;
	unsigned int pos_x;
	unsigned int error;
    };

    // resultset
    typedef std::multiset<ResultTuple,ResultTuple> ResultTupleSet;

    // resultset iterator
    typedef ResultTupleSet::iterator ResultTupleSetIterator; 

    // FLASM Edit distance
    ResultTupleSet flasm_ed ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error);

    // FLASM Hamming distance
    ResultTupleSet flasm_hd ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error);

}

#endif
