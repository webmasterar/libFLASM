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
#include <list>
#include <vector>
#include <libflasm.h>
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
	int              q_gram;          	// the q-gram length
	bool			return_all;
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
		q_gram                  = sw . q_gram;
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

	if ( num_seqs > 2 )
	{
        	fprintf( stderr, " Warning: %d sequences were read from file %s.\n", num_seqs, input_filename );
        	fprintf( stderr, " Warning: Only the first two (%s, %s) will be processed!\n", seq_id[0], seq_id[1] );
	}

	/* Check arguments sensible */
	unsigned int n = strlen ( ( char * ) seq[0] );
	unsigned int m = strlen ( ( char * ) seq[1] );

	unsigned int min_len = min(m,n);
	if ( q_gram > min_len )
	{
		fprintf( stderr, " Error: q-gram length cannot be longer than T or X!\n");
		return ( 1 );
	}
	else if ( q_gram < 2 )
	{
		fprintf( stderr, " Error: q-gram length too short!\n");
		return ( 1 );
	}

	if ( max_error >= q_gram )
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

	vector<unsigned char> * characters0 = new vector<unsigned char>();
	vector<unsigned char> * characters1 = new vector<unsigned char>();

	for( int i=0; i<n; i++) //work out alphabet size of sequence 1
	{
		if(find(characters0->begin(), characters0->end(), seq[0][i]) != characters0->end())  
		{
	    		continue;
		}	
		else characters0->push_back( seq[0][i] );
	}

	sort(characters0->begin(), characters0->end());


	for( int i=0; i<m; i++) //work out alphabet size of sequence 2
	{
		if(find(characters1->begin(), characters1->end(), seq[1][i]) != characters1->end())  
		{
	    		continue;
		}	
		else characters1->push_back( seq[1][i] );
	}

	unsigned int alphabet_size = characters1->size();
	
	unsigned char * seq0 = ( unsigned char * ) calloc ( ( n + 1 ) , sizeof( unsigned char ) );
	seq0[n] = '\0';
	unsigned int no_of_q_grams = pow(alphabet_size, q_gram );
	
	unsigned char * de_bruijn_seq = ( unsigned char  * ) calloc ( no_of_q_grams + q_gram  , sizeof( unsigned int ) );
	de_bruijn_seq[ 	no_of_q_grams + q_gram ] = '\0';

	unsigned char * db = ( unsigned char  * ) calloc ( alphabet_size*q_gram + 1  , sizeof( unsigned char ) );
	db[ alphabet_size*q_gram] = '\0';	

	for( int i=0; i< alphabet_size*q_gram ; i++)
		db[i] = '0';

	for(int j = 0; j<n; j++)
	{
		int pos = find(characters0->begin(), characters0->end(), seq[0][j]) - characters0->begin();
	    	seq0[j] = pos+'0';
	}

	deBruijn( alphabet_size, de_bruijn_seq, db, q_gram, 1, 1);
	
	free( db );

	int count = no_of_q_grams;
	for(int i=0; i<q_gram - 1; i++)
	{
		de_bruijn_seq[count] = de_bruijn_seq[i];
		count++;
	}

	unsigned int * errors = ( unsigned int * ) calloc ( no_of_q_grams + 1 , sizeof( unsigned int * ) );
	errors[ no_of_q_grams ] = '\0';

	for(int j = 0; j< no_of_q_grams; j++)
		errors[ j ] = no_of_q_grams + (q_gram - 1);

	unsigned char ** combinations = ( unsigned char ** ) calloc ( no_of_q_grams , sizeof( unsigned char * ) );

	for(int i = 0; i< no_of_q_grams; i++ )
	{
		combinations[i] = ( unsigned char * ) calloc ( ( q_gram  ) , sizeof( unsigned char ) );
	}

	unsigned char * q_gram_characters = ( unsigned char  * ) calloc ( ( alphabet_size + 1 ) , sizeof( unsigned char ) );
	q_gram_characters[ alphabet_size ] = '\0';

	for(int i = 0; i<alphabet_size; i++)
		q_gram_characters[i] = ( unsigned char ) i;

	unsigned int * index = ( unsigned int  * ) calloc ( ( q_gram + 1 ) , sizeof( int ) );
	index[ q_gram ] = '\0';

	permutations( q_gram_characters, alphabet_size, q_gram, index, 0, combinations); //obtain all q grams and place in array combinations

	free( index );
	free( q_gram_characters );

	list<MapTuple> ** mapList = ( list<MapTuple> ** ) calloc ( ( no_of_q_grams + 1 ) , sizeof( list<MapTuple> * ) );

	// create array of linked lists of tuples<string, int>
	for(int i=0; i<no_of_q_grams + 1; i++)
	{
		mapList[i] = new list<MapTuple>();
	}
	
	place_in_map( q_gram, no_of_q_grams, combinations, mapList ); // place all q grams in map

	ResultTupleSet results;

	if ( model == EDIT_DISTANCE )
	{
		results = flasm_ed ( seq0, n, de_bruijn_seq, no_of_q_grams + ( q_gram - 1), q_gram, max_error, true );
	}
	else
	{
		results = flasm_hd ( seq0, n, de_bruijn_seq, no_of_q_grams + ( q_gram - 1), q_gram, max_error, true );
	}

	free( seq0 );

	iterate_results( q_gram, no_of_q_grams, de_bruijn_seq, mapList, errors, results ); // iterate results to obtain minimum errors
	
	original_qgram_values( q_gram, no_of_q_grams, combinations, characters0 ); // Traverse characters0 to find original characters in sequence
	
	double end = gettime();

	/* Write results to output file */

	if ( ! ( out_fd = fopen ( output_filename, "w") ) )
	{
		fprintf ( stderr, " Error: Cannot open file %s!\n", output_filename );
		return ( 1 );
	}

	for(int i=0; i<no_of_q_grams; i++)
	{
		fprintf( out_fd, "(%s,%i)\n", combinations[i], errors[i] );
	}

	if ( fclose ( out_fd ) )
	{
		fprintf( stderr, " Error: file close error!\n");
		return ( 1 );
	}

	/* Give summary of results */
	

	fprintf( stderr, " q-gram length is %u\n", q_gram );
	fprintf( stderr, " Wrote %d q-grams to file %s\n", no_of_q_grams, output_filename );
	fprintf( stderr, " Elapsed time for LibFLASM: %lf secs\n", ( end - start ) );

	/* Free memory */

	for( i = 0; i< no_of_q_grams; i ++ )
	{
		free( combinations[i] );
		delete( mapList[i] );
	}
	free( combinations );
	free( mapList );

	for ( i = 0; i < num_seqs; i ++ )
	{
		free ( seq[i] );
		free ( seq_id[i] );
	}
	free ( seq );
	free ( seq_id );
	free ( sw . input_filename );
	free ( sw . output_filename );
	free( de_bruijn_seq );
	delete( characters0 );
	delete( characters1 );
	free( errors );
	

	return ( 0 );
}
