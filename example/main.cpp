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

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "main.h"

using namespace std;
using namespace libflasm;

int main( int argc, char **argv )
{

	struct TSwitch sw;

	FILE *           in_fd;                  // the input file descriptor
	FILE *           out_fd;                 // the input file descriptor
	int              model;                  // the model to use
	char *           input_filename;         // the input file name
	char *           output_filename;        // the output file name
	int              factor_length;          // the factor length
	int              max_error;              // the maximum edit distance
	unsigned char ** seq    = NULL;          // the sequence in memory
	unsigned char ** seq_id = NULL;          // the sequence id in memory

	unsigned int i;

	/* Decodes the arguments */

	i = decode_switches ( argc, argv, &sw );

	/* Check the arguments */

	if ( i < 5 )
	{
		usage ();
		return ( 1 );
	}
	else
	{
		model                   = sw . model;
		input_filename          = sw . input_filename;
		output_filename         = sw . output_filename;
		factor_length           = sw . factor_length;
		max_error               = sw . max_error;
	}

	/* Read the (Multi)FASTA file into memory */

	fprintf ( stderr, " Reading the (Multi)FASTA input file: %s\n", input_filename );
	if ( ! ( in_fd = fopen ( input_filename, "r") ) )
	{
		fprintf ( stderr, " Error: Cannot open file %s!\n", input_filename );
		return ( 1 );
	}

	char c;
	unsigned int num_seqs           = 0;
	unsigned int total_length       = 0;
	unsigned int max_alloc_seq_id   = 0;
	unsigned int max_alloc_seq      = 0;
	c = fgetc( in_fd );
	do
	{
		if ( c != '>' )
		{
			fprintf ( stderr, " Error: input file %s is not in FASTA format!\n", input_filename );
			return ( 1 );
		}
		else
		{
			if ( num_seqs >= max_alloc_seq_id )
			{
				seq_id = ( unsigned char ** ) realloc ( seq_id,   ( max_alloc_seq_id + ALLOC_SIZE ) * sizeof ( unsigned char * ) );
				max_alloc_seq_id += ALLOC_SIZE;
			}

			unsigned int max_alloc_seq_id_len = 0;
			unsigned int seq_id_len = 0;

			seq_id[ num_seqs ] = NULL;

			while ( ( c = fgetc( in_fd ) ) != EOF && c != '\n' )
			{
				if ( seq_id_len >= max_alloc_seq_id_len )
				{
					seq_id[ num_seqs ] = ( unsigned char * ) realloc ( seq_id[ num_seqs ],   ( max_alloc_seq_id_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
					max_alloc_seq_id_len += ALLOC_SIZE;
				}
				seq_id[ num_seqs ][ seq_id_len++ ] = c;
			}
			seq_id[ num_seqs ][ seq_id_len ] = '\0';

		}
		    if ( num_seqs >= max_alloc_seq )
		{
			seq = ( unsigned char ** ) realloc ( seq,   ( max_alloc_seq + ALLOC_SIZE ) * sizeof ( unsigned char * ) );
			max_alloc_seq += ALLOC_SIZE;
		}

		unsigned int seq_len = 0;
		unsigned int max_alloc_seq_len = 0;

		seq[ num_seqs ] = NULL;

		while ( ( c = fgetc( in_fd ) ) != EOF && c != '>' )
		{
			if( seq_len == 0 && c == '\n' )
			{
				fprintf ( stderr, " Omitting empty sequence in file %s!\n", input_filename );
				c = fgetc( in_fd );
				break;
			}
			if( c == '\n' || c == ' ' ) continue;

			c = toupper( c );

			if ( seq_len >= max_alloc_seq_len )
			{
				seq[ num_seqs ] = ( unsigned char * ) realloc ( seq[ num_seqs ],   ( max_alloc_seq_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
				max_alloc_seq_len += ALLOC_SIZE;
			}

			seq[ num_seqs ][ seq_len++ ] = c;

		}

		if( seq_len != 0 )
		{
			if ( seq_len >= max_alloc_seq_len )
			{
				seq[ num_seqs ] = ( unsigned char * ) realloc ( seq[ num_seqs ],   ( max_alloc_seq_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
				max_alloc_seq_len += ALLOC_SIZE;
			}
			seq[ num_seqs ][ seq_len ] = '\0';
			total_length += seq_len;
			num_seqs++;
		}

	} while( c != EOF );

	if ( fclose ( in_fd ) )
	{
		fprintf( stderr, " Error: file close error!\n");
		return ( 1 );
	}

	unsigned int n = strlen ( ( char * ) seq[0] );
	unsigned int m = strlen ( ( char * ) seq[1] );

	if ( num_seqs > 2 )
	{
        	fprintf( stderr, " Warning: %d sequences were read from file %s.\n", num_seqs, input_filename );
        	fprintf( stderr, " Warning: Only the first two (%s, %s) will be processed!\n", seq_id[0], seq_id[1] );
	}

	/* Check arguments sensible */

	unsigned int min_len = min(m,n);
	if ( factor_length > min_len )
	{
		fprintf( stderr, " Error: factor length cannot be longer than T or X!\n");
		return ( 1 );
	}
	else if ( factor_length < 2 )
	{
		fprintf( stderr, " Error: factor length too short!\n");
		return ( 1 );
	}

	if ( max_error >= factor_length )
	{
		fprintf( stderr, " Error: The number of errors is too high!\n");
		return ( 1 );
	}
	else if ( max_error < 0 )
	{
		fprintf( stderr, " Error: You cannot have fewer than 0 errors!\n");
		return ( 1 );
	}
	
	if ( ! ( model == EDIT_DISTANCE || model == HAMMING_DISTANCE ) )
	{
		fprintf( stderr, " Error: Invalid model used - please use 0 for Edit distance and 1 for Hamming distance!\n");
		return ( 1 );
	}

	/* Run the algorithm */

	double start = gettime();

	ResultTupleSet results;

	if ( model == EDIT_DISTANCE )
	{
		results = flasm_ed ( seq[0], n, seq[1], m, factor_length, max_error );
	}
	else
	{
		results = flasm_hd ( seq[0], n, seq[1], m, factor_length, max_error );
	}

	double end = gettime();

	/* Write results to output file */

	if ( ! ( out_fd = fopen ( output_filename, "w") ) )
	{
		fprintf ( stderr, " Error: Cannot open file %s!\n", output_filename );
		return ( 1 );
	}

	fprintf( out_fd, "#(end_pos_t,end_pos_x,error)\n" );

	ResultTupleSetIterator it;
	for ( it = results.begin(); it != results.end(); ++it )
	{
		ResultTuple res = *it;
		fprintf( out_fd, "(%u,%u,%u)\n", res.pos_t, res.pos_x, res.error );
	}

	if ( fclose ( out_fd ) )
	{
		fprintf( stderr, " Error: file close error!\n");
		return ( 1 );
	}

	/* Give summary of results */

	fprintf( stderr, " Seq T id is %s and its length is %u\n", seq_id[0], n );
	fprintf( stderr, " Seq X id is %s and its length is %u\n", seq_id[1], m );
	fprintf( stderr, " Wrote %d positions to file %s\n", (int) results.size(), output_filename );
	fprintf( stderr, " Elapsed time for LibFLASM: %lf secs\n", ( end - start ) );

	/* Free memory */

	for ( i = 0; i < num_seqs; i ++ )
	{
		free ( seq[i] );
		free ( seq_id[i] );
	}
	free ( seq );
	free ( seq_id );
	free ( sw . input_filename );
	free ( sw . output_filename );

	return ( 0 );
}
