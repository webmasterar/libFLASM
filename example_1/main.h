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

#include <libflasm.h>

#define ALLOC_SIZE 		1048576
#define EDIT_DISTANCE 		0
#define HAMMING_DISTANCE 	1

struct TSwitch
{
    int                 model;                  // the model to use. 0 = edit distance, 1 = Hamming distance
    char *              input_filename;         // the input file name
    char *              output_filename;        // the output file name
    int                 factor_length;          // the length of the factor
    int                 max_error;              // the maximum mismatches allowed in a match
    bool                return_all;             // if true all matches are returned
};

double gettime ( void );
int decode_switches ( int argc, char * argv [], struct TSwitch * sw );
void usage ( void );
