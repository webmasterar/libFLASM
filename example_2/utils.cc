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
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

#include "main.h"
#include <libflasm.h>

static struct option long_options[] =
{
   { "model",                   required_argument, NULL, 'm' },
   { "input-file",              required_argument, NULL, 'i' },
   { "output-file",             required_argument, NULL, 'o' },
   { "q-gram",           	required_argument, NULL, 'q' },
   { "max-error",               required_argument, NULL, 'k' },
   { "help",                    no_argument,       NULL, 'h' },
   {  NULL,                     0,                 NULL,  0  }
};


/* 
Decode the input switches 
*/
int decode_switches ( int argc, char * argv [], struct TSwitch * sw )
{
   int          oi;
   int          opt;
   double       val;
   char       * ep;
   int          args;

   /* initialisation */
   sw -> input_filename     = NULL;
   sw -> output_filename    = NULL;
   sw -> q_gram      	    = 0;
   sw -> return_all         = false;
   sw -> max_error          = 0;
   sw -> model              = 0;
   args = 0;

   while ( ( opt = getopt_long ( argc, argv, "m:i:o:q:k:h", long_options, &oi ) ) != - 1 )
    {
      switch ( opt )
       {
         case 'm':
           sw -> model = atoi ( optarg );
           args ++;
           break;

         case 'i':
           sw -> input_filename = ( char * ) malloc ( ( strlen ( optarg ) + 1 ) * sizeof ( char ) );
           strcpy ( sw -> input_filename, optarg );
           args ++;
           break;

         case 'o':
           sw -> output_filename = ( char * ) malloc ( ( strlen ( optarg ) + 1 ) * sizeof ( char ) );
           strcpy ( sw -> output_filename, optarg );
           args ++;
           break;

         case 'q':
           sw -> q_gram = atoi ( optarg );
           args ++;
           break;

         case 'k':
           sw -> max_error = atoi ( optarg );
           args ++;
           break;

         case 'h':
           return ( 0 );
       }
    }

   if ( args < 5 )
   {
       usage ();
       exit ( 1 );
   }
   else
   {
       return ( optind );
   }
}


/* 
Usage of the tool 
*/
void usage ( void )
{
    fprintf ( stdout, " dbflasm <options>\n" );
    fprintf ( stdout, " Available arguments:\n" );
    fprintf ( stdout, "  -m, --model                 <int>     The distance model. 0 = Edit distance, 1 = Hamming distance.\n" );
    fprintf ( stdout, "  -i, --input-file            <file>    (Multi)FASTA input filename containing text T and pattern X.\n" );
    fprintf ( stdout, "  -o, --output-file           <file>    Output filename for the positions.\n" );
    fprintf ( stdout, "  -q, --q-gram-length         <int>     The length of the q-grams.\n" );
    fprintf ( stdout, "  -k, --max-error             <int>     The maximum error (edit/Hamming distance) permitted in a match.\n" );
}


/*
Get time for timer
*/
double gettime( void )
{
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
}

