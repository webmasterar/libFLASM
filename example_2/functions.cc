
#include <iostream>
#include <list>
#include "main.h"

using namespace std;
using namespace libflasm;

void place_in_map( unsigned int q_gram, unsigned int no_of_q_grams, unsigned char ** combinations, list<MapTuple> ** mapList )
{
	for(unsigned int j=0; j<no_of_q_grams; j++)
	{	
		int count = 0;
		unsigned int hashValue = 0;
		for(int k =q_gram-1; k>=0; k--)
		{
			hashValue = hashValue + pow( 2, combinations[j][k]-'0'  );
		}	
		hashValue = hashValue % ( no_of_q_grams + 1 );

		MapTuple val = { (unsigned char * ) combinations[j], j };

		mapList[hashValue]->push_back( val );
	}
}

void iterate_results(unsigned int q_gram, unsigned int no_of_q_grams,  unsigned char * de_bruijn_seq, list<MapTuple> ** mapList, unsigned int * errors, ResultTupleSet results)
{
	ResultTupleSetIterator it;
	for ( it = results.begin(); it != results.end(); ++it )
	{
		ResultTuple res = *it;

		unsigned char * q_gram_in_db = ( unsigned char * ) calloc ( ( q_gram ) , sizeof( unsigned char ) );
		int counter = 0;

		for(int j=res.pos_x - ( q_gram - 1 ); j<=res.pos_x; j++ )
		{
		
			q_gram_in_db[counter] = de_bruijn_seq[j];
			counter++;
		}
		
		
		unsigned int hashValueCalc = 0;
		for(int k =q_gram-1; k>=0; k--)
		{
			hashValueCalc = hashValueCalc + pow( 2, q_gram_in_db[k]  - '0' );	
		}	
		hashValueCalc = hashValueCalc % ( no_of_q_grams + 1 );

		int error_index = 0;
		list<MapTuple>::iterator iter;
		for (iter = mapList[hashValueCalc]->begin(); iter != mapList[hashValueCalc]->end(); ++iter) 
		{
			
   			MapTuple mapResult = *iter;


			unsigned int mVal = atoi( ( const char * ) mapResult.value );
			unsigned int qVal = atoi( ( const char * ) q_gram_in_db );

			
		        if( mVal == qVal )
			{	
				error_index = mapResult.index;
			}
			else continue;
		}

		if( res . error < errors[ error_index ] )
			errors[ error_index ] = res . error;

		free( q_gram_in_db );
	}
}

void original_qgram_values( unsigned int q_gram, unsigned int no_of_q_grams, unsigned char ** combinations, vector<unsigned char> * characters0 )
{
	
	for(int i =0; i<no_of_q_grams; i++)
	{
		for(int j =0; j<q_gram; j++)
		{
			unsigned char originalChar =  characters0->at( (combinations[i][j] )-'0' );
			combinations[i][j] = originalChar;
		}
	}
}
