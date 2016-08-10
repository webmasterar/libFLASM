#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main.h"

using namespace std;

void deBruijn(unsigned int alphabet_size, unsigned char * de_bruijn_seq, unsigned char * db, unsigned int factor_length, unsigned int t, unsigned int p) 
{

	static int count = 0;

	if ( t > factor_length )
	{
		if ( factor_length % p == 0 )
		{
			for(int i= 1; i < p+1;  i++)
			{

                		de_bruijn_seq[count] = db[i];
				count++;
			}
		}
	}
        else
	{
		db[t] =  db[t - p];
		deBruijn(alphabet_size, de_bruijn_seq, db, factor_length, t+1, p);

		for ( int j= db[t-p]-'0' + 1;  j< alphabet_size; j++)
		{
		        db[t] = j+'0';
		        deBruijn(alphabet_size, de_bruijn_seq, db, factor_length, t+1, t);
		}
	}
}
