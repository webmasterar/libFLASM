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

#ifndef __MAXSHIFTMN__
#define __MAXSHIFTMN__

//#include "maxshiftm/maxshiftm.h"

using namespace maxshiftm;

class MaxShiftMn : public MaxShiftM
{
    private:
	unsigned int max_k;
	
    public:
	MaxShiftMn ( unsigned char * t, unsigned int n, unsigned char * p, unsigned int m, unsigned int factor_length, unsigned int max_error );
	ResultTupleSet run ();
};

#endif
