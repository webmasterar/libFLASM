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

#include <stdio.h>
#include <stdlib.h>
#include "libflasm.h"

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
ResultTupleSet flasm_ed ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error )
{
	unsigned char * h;
	h = ( unsigned char * ) calloc ( factor_length + 1, sizeof ( unsigned char ) );

	int k = -max_error;
	unsigned int error = 0;
	unsigned int pos_t = 0;
	unsigned int pos_x = 0;

	ResultTupleSet results;

	CharString haystack = (CharString) t;
	CharString needle;
	Finder<CharString> finder( haystack );
	Pattern<CharString, Myers<>> pattern;

	unsigned int i;
	for ( i = 0; i < m - factor_length + 1; i++ )
	{
		memcpy( &h[0], &x[i], factor_length );
		h[factor_length] = '\0';

		needle = (CharString) h;

		setNeedle( pattern, needle );

		while ( find( finder, pattern, k ) )
		{
			pos_t = (unsigned int) endPosition( finder ) - 1;

			pos_x = i + factor_length - 1;

			error = (unsigned int) abs( getScore( pattern ) );

			ResultTuple match = {pos_t, pos_x, error};

			results.insert( match );
		}
		
		clear( finder );
		goBegin ( finder );

	}

	free ( h );

	return results;
}

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
ResultTupleSet flasm_hd ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error )
{
	ResultTupleSet results;

	//initialise 2 line matrix
    	WORD * M0;
    	WORD * M1;
	WORD y; 

	unsigned int  err, i, j;
	if ( ( M0 = ( WORD * ) calloc ( ( m + 1 ) , sizeof ( WORD  ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M0 could not be allocated!\n");
		return results;
	}
	if ( ( M1 = ( WORD * ) calloc ( ( m + 1 ) , sizeof ( WORD ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M1 could not be allocated!\n");
		return results;
	}
	y = ( ( WORD ) ( 1 ) << ( factor_length - 1 ) ) - 1;
   
	//loop through sequences
    	for ( i = 0; i < n + 1; i++ ) //loop through t
    	{
        	for ( j = 1; j < m + 1; j++ ) //loop through p
        	{
			switch ( i % 2 ) 
			{
				case 0:

				if ( i == 0 )
					M0[j] = ( ( WORD ) 2 << ( min ( j , factor_length ) - 1 ) ) - 1;
				else
					M0[j] = shift_c ( M1[j - 1], y ) | _delta ( t[i - 1], x[j - 1] );

                    		if ( j >= factor_length && i >= factor_length )
                    		{                
                        		err = popcount_words ( M0[j] );

                        		if ( err <= max_error )
                        		{
			    			ResultTuple match = { i - 1, j - 1, err };
                            			results.insert ( match );
                        		}
                    		}
                    		break;

                		case 1:

				if( i == 0 )
					M1[j] = ( ( WORD ) 2 << ( min ( j , factor_length ) - 1 ) ) - 1;
				else
					M1[j] = shift_c ( M0[j - 1], y ) | _delta ( t[i - 1], x[j - 1] );

                    		if ( j >= factor_length && i >= factor_length )
                    		{                
                        		err = ( unsigned int ) popcount_words ( M1[j] );

                        		if ( err  <= max_error )
                        		{
			    			ResultTuple match = { i - 1, j - 1, err };
                            			results.insert ( match );
                        		}
                    		}
                    		break;
            		}
		}
    	}

    	free ( M0 );
    	free ( M1 );
    	
	return results;
}


inline WORD _shift ( WORD a ) 
{
	return ( ( WORD ) a << 1 );
}

inline WORD shift_c( WORD a, WORD x ) 
{
	return _shift ( a & x );
}

unsigned int popcount_words ( WORD words )
{
	unsigned int count = __builtin_popcountl ( words );
  
	return count;
}

inline unsigned int _delta( char a, char b )
{
	if( a == b )			
		return 0;
	else					
		return 1;
}
