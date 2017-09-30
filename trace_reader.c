#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int trace_reader(int argc, char* argv[]){
	assert(argv[1]!=NULL);
	FILE *filepntr;
	filepntr = fopen(argv[1], "r");
	int retVal;
	int mem_address;

	while((retVal=fscan(filepntr, "%x", &mem_address))!=EOF){
		return mem_address;
	}

	fclose(filepntr);
	free(retVal);
	free(mem_address);
}