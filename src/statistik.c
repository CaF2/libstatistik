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

	containing all functions of the stati_t struct and the stati_list_t struct
	statistik = miss spelling of swedish statestik = statistics.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "statistik.h"

#ifndef NO_ALLOC

/**
	Init the stati_t struct
	
	@return 
		returns a new stati_t struct
*/
stati_t *stati_init(size_t preAllocSize)
{
	
	stati_t *ret=calloc(1,sizeof(stati_t));
	
	if(preAllocSize>0)
	{
		ret->varcount=malloc(preAllocSize*sizeof(stati_list_t));
		ret->vars=malloc(preAllocSize*sizeof(double));
		ret->preAllocSize=preAllocSize;
	}
	
	return ret;
}

/**
	Destroys/frees the stati_t struct
	
	@param yourItems 
		the object to destroy
*/
void stati_destroy(stati_t *yourItems)
{
	free(yourItems->vars);
	free(yourItems->varcount);
	free(yourItems);
}


#else
///<GLOBAL BUFFER is a temporary char that contains the information for the output.
static char *GLOBAL_BUFFER=(char[GLOBAL_STRING_ALLOC_SIZE]){0};

/**
	In no malloc mode, the destroy function will just reset the value
*/
void stati_destroy(stati_t *yourItems)
{
	memset(yourItems->vars,0,sizeof(double)*yourItems->amount);
	memset(yourItems->varcount,0,sizeof(stati_list_t)*yourItems->countamount);
	memset(yourItems,0,sizeof(stati_t));
}

#endif

/**
	Inserts an item into the object **USE THIS CAREFULLY**

	@param yourItems 
		the object you want to insert the item into
	@param item
		the parameter to insert
*/
void stati_insert_count_item(stati_t *yourItems,double item)
{
	for(size_t i=0;i<yourItems->countamount;i++)
	{
		if(yourItems->varcount[i].var==item)
		{
			yourItems->varcount[i].amount++;
			return;
		}
	}
	
	if(yourItems->preAllocSize<(yourItems->countamount+1))
	{
		#ifndef NO_ALLOC
		yourItems->varcount=realloc(yourItems->varcount,(yourItems->countamount+1)*sizeof(stati_list_t));
		#else
		stati_print("ERROR (OVERFLOW) IN : stati_insert_count_item\n");
		return;
		#endif
	}
	
	yourItems->varcount[yourItems->countamount].amount=1;
	yourItems->varcount[yourItems->countamount].var=item;
	
	yourItems->countamount++;
}

/**
	Add an item generally to an object, use this when you want to insert an item

	@param yourItems 
		the object you want to insert the item into
	@param item
		the parameter to insert
*/
void stati_add(stati_t *yourItems, double item)
{
	if(yourItems->preAllocSize<(yourItems->amount+1))
	{
		#ifndef NO_ALLOC
		yourItems->vars=realloc(yourItems->vars,(yourItems->amount+1)*sizeof(double));
		#else
		stati_print("ERROR (OVERFLOW) IN : stati_add\n");
		return;
		#endif
	}
	
	yourItems->vars[yourItems->amount]=item;
	yourItems->amount++;
	
	stati_insert_count_item(yourItems,item);
}

/**
	Sorts the structure, so it wont print nicely at the end, also faster for eq pdf or cdf.
	Use this function at the end
	

	@param yourItems 
		the object you want to sort
*/
void stati_sort(stati_t *yourItems)
{
	for(size_t i=0;i<yourItems->amount;i++)
	{
		for(size_t j=0;j<yourItems->amount;j++)
		{
			if(yourItems->vars[i]<yourItems->vars[j])
			{
				double tmp=yourItems->vars[i];
				yourItems->vars[i]=yourItems->vars[j];
				yourItems->vars[j]=tmp;
			}
		}
	}
	
	for(size_t i=0;i<yourItems->countamount;i++)
	{
		for(size_t j=0;j<yourItems->countamount;j++)
		{
			if(yourItems->varcount[i].var<yourItems->varcount[j].var)
			{
				double tmpVar=yourItems->varcount[i].var;
				size_t tmpAmount=yourItems->varcount[i].amount;
				yourItems->varcount[i].var=yourItems->varcount[j].var;
				yourItems->varcount[i].amount=yourItems->varcount[j].amount;
				yourItems->varcount[j].var=tmpVar;
				yourItems->varcount[j].amount=tmpAmount;
			}
		}
	}
}

/**
	Calculates the mean of an object
	
	@param yourItems 
		the object get the mean from
	@return 
		returns the mean
*/
double stati_mean(stati_t *yourItems)
{
	double out=0;
	
	for(size_t i=0;i<yourItems->amount;i++)
	{
		out+=yourItems->vars[i];
	}
	
	return out/(double)yourItems->amount;
}

/**
	Calculates the mean(for all objs^2) of an object.
	Only used by the sigma function
	
	@param yourItems 
		the object to do this for
	@return 
		returns the mean(for all objs^2)
*/
double stati_mean_square(stati_t *yourItems)
{
	double out=0;
	
	for(size_t i=0;i<yourItems->amount;i++)
	{
		out+=yourItems->vars[i]*yourItems->vars[i];
	}
	
	return out/(double)yourItems->amount;
}

/**
	Calculates the standard variation of an object
	
	@param yourItems 
		the object to calculate the standard variation
	@param allDataCollected
		1=all data is collected, treats it as all data is collected, also an exact result, 0= gives an approximation.
	@return 
		returns the standard variation
*/
double stati_sigma(stati_t *yourItems,uint8_t allDataCollected)
{
	double estimate=!allDataCollected?((double)yourItems->amount)/((double)yourItems->amount-1.0):1;
	
	double mean=stati_mean(yourItems);
	
	return sqrt(estimate*(stati_mean_square(yourItems)-(mean*mean)));
}

/**
	Get the lowest value in the object structure
	
	@param yourItems 
		the object
	@return 
		returns the lowest number
*/
double stati_get_lowest(stati_t *yourItems)
{
	double currLowest=DBL_MAX;
	for(size_t i=0;i<yourItems->amount;i++)
	{
		if(yourItems->vars[i]<currLowest)
			currLowest=yourItems->vars[i];
	}
	
	return currLowest;
}

/**
	Get the highest value in the object structure
	
	@param yourItems 
		the object
	@return 
		returns the highest number
*/
double stati_get_highest(stati_t *yourItems)
{
	double currHighest=-DBL_MAX;
	for(size_t i=0;i<yourItems->amount;i++)
	{
		if(yourItems->vars[i]>currHighest)
			currHighest=yourItems->vars[i];
	}
	
	return currHighest;
}

/**
	Calculates the pdf of the structure
	
	@param yourItems 
		the object
	@param x
		the position where to get the pdf
	@return 
		returns the probability
*/
double stati_pdf(stati_t *yourItems,double x)
{
	for(size_t i=0;i<yourItems->countamount;i++)
	{
		if(yourItems->varcount[i].var==x)
		{
			return (double)yourItems->varcount[i].amount/yourItems->amount;
		}
	}
	
	return 0.0;
}

/**
	Calculates the cdf of the structure
	
	@param yourItems 
		the object
	@param x
		the position where to get the cdf
	@return 
		returns the probability
*/
double stati_cdf(stati_t *yourItems,double x)
{
	double percent=0;

	for(size_t i=0;i<yourItems->countamount;i++)
	{
		if(yourItems->varcount[i].var<=x)
		{
			percent+=(double)yourItems->varcount[i].amount/yourItems->amount;
		}
	}
	
	return percent;
}

/**
	Prints the table of the structure, with {object, amount, pdf, cdf}
	
	@param yourItems 
		the object
	@return 
		returns the string of the output
*/
char *stati_print_table(stati_t *yourItems)
{
	char tmpstr[100];
	#ifndef NO_ALLOC
	char *out=NULL;
	#else
	char *out=GLOBAL_BUFFER;
	#endif
	size_t outpos=0;

	const char *listHead="OBJECT,AMOUNT,PDF,CDF\n";
	
	outpos=strlen(listHead);
	
	#ifndef NO_ALLOC
	if((out=malloc(outpos*sizeof(char)))==NULL)
		printf("ERROR ON MALLOC IN : %s LINE: %d\n",__func__,__LINE__);
	#endif
	memcpy(out,listHead,outpos);
	
	for(size_t i=0;i<yourItems->countamount;i++)
	{
		stati_list_t *obj=&yourItems->varcount[i];
		
		sprintf(tmpstr,"%f,%ld,%f,%f\n",obj->var,obj->amount,(double)obj->amount/yourItems->amount,stati_cdf(yourItems,obj->var));
		
		size_t tmpstrlen=strlen(tmpstr);
		
		if(i+1<yourItems->countamount)
		{
			#ifndef NO_ALLOC
			if((out=realloc(out,tmpstrlen+outpos))==NULL)
				printf("ERROR ON REALLOC IN : %s LINE: %d\n",__func__,__LINE__);
			#endif
			memcpy(out+outpos,tmpstr,tmpstrlen);
			outpos+=tmpstrlen;
		}
		else
		{
			#ifndef NO_ALLOC
			if((out=realloc(out,tmpstrlen+outpos+1))==NULL)
				printf("ERROR ON REALLOC IN : %s LINE: %d\n",__func__,__LINE__);
			#endif
			memcpy(out+outpos,tmpstr,tmpstrlen);
			out[tmpstrlen+outpos]='\0';
		}
	}
	
	return out;
}

/**
	Prints all thinkable {object, amount, pdf, cdf}, amount, lowest, highest, mean, standard variation.
	
	@param yourItems 
		the object
	@param isComplete
		if all data is collected, or if its a portion of all collected data (only used by standard variation)
	@return 
		returns the string of the output
*/
char *stati_print_all(stati_t *yourItems,uint8_t isComplete)
{
	char tmpstr[200];
	char *out;
	
	out=stati_print_table(yourItems);
	
	//printf("%s",out);
	
	sprintf(tmpstr,"\nTOTAL AMOUNT=%ld, LOWEST=%f, HIGHEST=%f, MEAN=%f, SIGMA=%f\n",yourItems->amount,stati_get_lowest(yourItems),stati_get_highest(yourItems),stati_mean(yourItems),stati_sigma(yourItems,isComplete));
	
	size_t tablelen=strlen(out);
	size_t misclen=strlen(tmpstr);
	
	#ifndef NO_ALLOC
	if((out=realloc(out,tablelen+strlen(tmpstr)+1))==NULL)
		printf("ERROR ON REALLOC IN : %s\n",__func__);
	#endif
		
	memcpy(out+tablelen,tmpstr,misclen);
	out[tablelen+misclen]='\0';
	
	return out;
}

/**
	Dumps all thinkable data to a file
	
	@param yourItems
		the object
	@param isComplete
		if all data is collected, or if its a portion of all collected data (only used by standard variation)
	@param filename
		the filename of the file you want to dump everthing to.
*/
void stati_dump_to_file(stati_t *yourItems, uint8_t isComplete, const char *filename)
{
	FILE *fp;
	
	fp = fopen(filename, "w+");
	
	char *outtxt=stati_print_all(yourItems,isComplete);

	if(fp!=NULL && outtxt!=NULL)
	{
		fwrite(outtxt,sizeof(char),strlen(outtxt),fp);
	}
	else
	{
		printf("Could not write to file!");
	}

	free(outtxt);	
	fclose (fp);
}
