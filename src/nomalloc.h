/**
	MAIN CONFIGURATION FILE FOR THE STATISTIK LIBRARY
*/

//this value should be YOUR_ALLOCATED_SIZE*40 + 120, 40 should be increased depending on the maximum value, eg 5000000.123 will generate more text than 5.123
#define GLOBAL_STRING_ALLOC_SIZE 500

//for printing it out to the user.
#define stati_print(str) puts(str)
