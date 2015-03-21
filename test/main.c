#include <stdio.h>
#include <string.h>
#include <math.h>

#include "statistik.h"

int main()
{
	stati_t *myStats=stati_init(200);
	
	stati_add(myStats,1);
	stati_add(myStats,5);
	stati_add(myStats,5);
	stati_add(myStats,96);
	stati_add(myStats,3);
	stati_add(myStats,2);
	stati_add(myStats,6);
	stati_add(myStats,2);
	stati_add(myStats,100);
	stati_add(myStats,0);
	stati_add(myStats,96);
	stati_add(myStats,-96);
	
	for(int i=0;i<=100;i++)
	{
		stati_add(myStats,i);
	}
	
	stati_sort(myStats);
	
	char *out=stati_print_all(myStats,0);
	printf("%s",out);
	free(out);

/*	stati_dump_to_file(myStats,0,"test.txt");*/
	
	stati_destroy(myStats);

	return 0;
}
