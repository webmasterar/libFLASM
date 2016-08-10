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

#include "libflasm.h"

using namespace libflasm;

#define ALLOC_SIZE 		1048576
#define EDIT_DISTANCE 		0
#define HAMMING_DISTANCE 	1

struct TSwitch
{
    int                 model;                  // the model to use. 0 = edit distance, 1 = Hamming distance
    char *              input_filename;         // the input file name
    char *              output_filename;        // the output file name
    int                 q_gram;		        // the length of the q-grams
    int                 max_error;              // the maximum mismatches allowed in a match
    bool                return_all;             // if true all matches are returned
};


struct MapTuple 
{
	unsigned char * value;
	unsigned int index;
};


double gettime ( void );
int decode_switches ( int argc, char * argv [], struct TSwitch * sw );
void usage ( void );
void deBruijn(unsigned int alphabet_size, unsigned char * de_bruijn_seq, unsigned char * db, unsigned int factor_length, unsigned int t, unsigned int p);
void permutations(unsigned char * s, int alphabet_size, int q_gram, unsigned int * index, int depth,  unsigned char ** combinations);
void place_in_map( unsigned int q_gram, unsigned int no_of_q_grams, unsigned char ** combinations, list<MapTuple> ** mapList );
void iterate_results(unsigned int q_gram, unsigned int no_of_q_grams,  unsigned char * de_bruijn_seq, list<MapTuple> ** mapList, unsigned int * errors, ResultTupleSet results);
void original_qgram_values( unsigned int q_gram, unsigned int no_of_q_grams, unsigned char ** combinations, vector<unsigned char> * characters0 );
