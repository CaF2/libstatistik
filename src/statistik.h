/**
	@file
	@author Florian Evaldsson
	@version 0.1
	
	@section LICENSE
	
	Copyright (c) 2015 Florian Evaldsson <florian.evaldsson@telia.com>


	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
	@section DESCRIPTION

	the main header of the statistik library
*/
#pragma once

#include <stdint.h>

#ifndef NO_ALLOC
#include <stdlib.h>
#else
#include "nomalloc.h"
#define free(abs) NULL
#endif

typedef struct stati_list_t
{
	double var;
	size_t amount;
}stati_list_t;

typedef struct stati_t
{
	//antalet variabler lista {1,4,3,4,-1,-1}
	double *vars;
	//antalet variabler ex 6
	size_t amount;
	
	//antalet av de olika elementen, ex 4-> 2 1->1 osv
	stati_list_t *varcount;
	size_t countamount;
	
	size_t preAllocSize;
}stati_t;

#ifndef NO_ALLOC

stati_t *stati_init(size_t preAllocSize);
#define stati_free(obj) free(obj)

#else

#define stati_init(size) (&(stati_t){.preAllocSize=size,.varcount=((stati_list_t[size]){{0}}),.vars=((double[size]){0})})
#define free(obj)

#endif

void stati_destroy(stati_t *yourItems);
void stati_insert_count_item(stati_t *yourItems,double item);
void stati_add(stati_t *yourItems, double item);
void stati_sort(stati_t *yourItems);
double stati_mean(stati_t *yourItems);
double stati_mean_square(stati_t *yourItems);
double stati_sigma(stati_t *yourItems,uint8_t allDataCollected);
double stati_get_lowest(stati_t *yourItems);
double stati_get_highest(stati_t *yourItems);
double stati_pdf(stati_t *yourItems,double x);
double stati_cdf(stati_t *yourItems,double x);
char *stati_print_table(stati_t *yourItems);
char *stati_print_all(stati_t *yourItems,uint8_t isComplete);
void stati_dump_to_file(stati_t *yourItems, uint8_t isComplete, const char *filename);

#ifdef NO_LINKING
	#include "statistik.c"
#endif
