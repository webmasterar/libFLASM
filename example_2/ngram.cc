#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "main.h"

void permutations(unsigned char * s, int alphabet_size, int q_gram, unsigned int * index, int depth, unsigned char ** combinations)
{
    static int count = 0;
    if (depth == q_gram)
    {   
        for (int i = 0; i < q_gram; ++i)
        {
	    combinations[count][i] = s[index[i]]+'0';
	    
        }	
	count++;
        return;
    }
   
    for (int i = 0; i <alphabet_size ; ++i)
    {
        index[depth] =  i;
        permutations(s, alphabet_size, q_gram, index, depth+1, combinations);
    }

}

