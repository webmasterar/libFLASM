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
#include "maxshiftmn.h"

MaxShiftMn::MaxShiftMn ( unsigned char * t, unsigned int n, unsigned char * p, unsigned int m, unsigned int factor_length, unsigned int max_error ) : MaxShiftM ( p, m, t, n, factor_length )
{
    this->max_k = max_error;
}

ResultTupleSet MaxShiftMn::run ()
{
    ResultTupleSet results;
    
    unsigned int i, j, k, err;

    WORD * ones;
    if ( ( ones = ( WORD * ) calloc ( this->lim.words , sizeof ( WORD ) ) ) == NULL )
    {
        fprintf( stderr, " Error: ow could not be allocated!\n");
        return results;
    }

    //initialise 2 line matrix
    WORD ** M0;
    WORD ** M1;
    if ( ( M0 = ( WORD ** ) calloc ( ( this->n + 1 ) , sizeof ( WORD * ) ) ) == NULL )
    {
        fprintf( stderr, " Error: M0 could not be allocated!\n");
        return results;
    }
    if ( ( M1 = ( WORD ** ) calloc ( ( this->n + 1 ) , sizeof ( WORD * ) ) ) == NULL )
    {
        fprintf( stderr, " Error: M1 could not be allocated!\n");
        return results;
    }
    for ( j = 0; j < n + 1; j++ )
    {
        if ( ( M0[j] = ( WORD * ) calloc ( this->lim.words , sizeof ( WORD ) ) ) == NULL )
        {
            fprintf( stderr, " Error: M0J could not be allocated!\n");
            return results;
        }
        if ( ( M1[j] = ( WORD * ) calloc ( this->lim.words , sizeof ( WORD ) ) ) == NULL )
        {
            fprintf( stderr, " Error: M1J could not be allocated!\n");
            return results;
        }
    }

    //loop through sequences
    for ( i = 1; i < this->m + 1; i++ ) //loop through p
    {
        
        for ( j = 0; j < this->n + 1; j++ ) //loop through t
        {

            //make ones
            if ( j == 0  && i <= this->lim.h ) {
                shift_words ( ones );
                ones[this->lim.words - 1] = ones[this->lim.words - 1] + 1;
            }

            switch ( i % 2 ) {

                case 0:

                    if ( j == 0 )
                    {
                        //fill up the first column with ones up to length h
                        memcpy ( M1[j], ones, this->lim.words * sizeof ( WORD ) );
                    }
                    else
                    {
                        //copy values from diagonal cell into current cell
                        for ( k = 0; k < this->lim.words; k++ ) {
                            M1[j][k] = M0[j - 1][k];
                        }

                        //shift things along one and clear left most bit
                        shiftc_words ( M1[j] );

                        //set last bit on right to hamming distance of the characters
                        M1[j][this->lim.words - 1] = M1[j][this->lim.words - 1] | delta ( this->p[i - 1], this->t[j - 1] );
                    }

                    if ( j >= this->lim.h && i >= this->lim.h )
                    {                
                        err = popcount_words ( M1[j] );

                        if ( err <= this->max_k )
                        {
			    ResultTuple match = { j - 1, i - 1, err };
                            results.insert ( match );
                        }
                    }
                    break;

                case 1:

                    if ( j == 0 )
                    {
                        //fill up the first column with ones up to length h
                        memcpy ( M0[j], ones, this->lim.words * sizeof ( WORD ) );
                    }
                    else
                    {
                        //copy values from diagonal cell into current cell
                        for ( k = 0; k < this->lim.words; k++ ) {
                            M0[j][k] = M1[j - 1][k];
                        }

                        //shift things along one and clear left most bit
                        shiftc_words ( M0[j] );

                        //set last bit on right to hamming distance of the characters
                        M0[j][this->lim.words - 1] = M0[j][this->lim.words - 1] | delta ( this->p[i - 1], this->t[j - 1] );
                    }

                    if ( j >= this->lim.h && i >= this->lim.h )
                    {                
                        err = popcount_words ( M0[j] );

                        if ( err  <= this->max_k )
                        {
			    ResultTuple match = { j - 1, i - 1, err };
                            results.insert ( match );
                        }
                    }
                    break;
            }

        }

    }

    for ( j = 0; j < this->n + 1; j++ ) {
        free ( M0[j] );
        free ( M1[j] );
    }
    free ( M0 );
    free ( M1 );
    free ( ones );
    
    return results;
}
