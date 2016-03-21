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

multiset<ResultTuple,ResultTuple> flasm ( unsigned char * t, unsigned int n, unsigned char * x, unsigned int m, unsigned int factor_length, unsigned int max_error )
{
	unsigned char * h;
	h = ( unsigned char * ) calloc ( factor_length + 1, sizeof ( unsigned char ) );

	int k = -max_error;
	unsigned int error = 0;
	unsigned int pos_t = 0;
	unsigned int pos_x = 0;

	multiset<ResultTuple,ResultTuple> results;

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

			pos_t = ( unsigned int ) endPosition( finder );

			pos_x = i + factor_length;

			error = ( unsigned int ) abs( getScore( pattern ) );

			ResultTuple result = {pos_t, pos_x, error};

			results.insert( result );

		}

		goBegin ( finder );

	}

	free ( h );

	return results;
}
