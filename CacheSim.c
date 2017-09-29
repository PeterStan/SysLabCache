#include "Cache.h"
//#include "CacheTest.c"


//takes trace file and returns hit rate
int readTrace(char *file){
	 
	unsigned int address;
	FILE *fp;
	fp = fopen(file, "r+");

	while( fscanf(fp, "%X", &address) != EOF){
		accessCache(address);//printf("%X\n", address);
	}

	fclose(fp);
}

//Takes the base-two log of the address passed 
int logBaseTwo(int quantity){
	assert(quantity>0);
	int x = 0;
	int quotient = quantity/(2^x);
	while(quotient>1){
		x++;
		quotient = quantity/(2^x);
	}
	return x;
}

//Outputs the cache set in which the address falls
int whichSet(int address){
	return 0;
}

int whichSetTest(){
	//assert(whichSet() == 0);
	return 0;
};

//returns 0 or 1 based on wether it is a hit or miss
int accessCache(int address){

	//find way, 
	//check tag
	//run either update on hit or update on miss




	return 0;
}

	//argv[1] = set associativity
	//argv[2] = line size in bytes
	//argv[3] = total cache size in kbytes
int buildCache(int k, int l, int c){
	struct cache Cache;
	Cache.setNumFieldLength = setIndexLength(k,l,c);
	Cache.blockOffsetFieldLength = offsetLength(k,l,c);
	Cache.tagFieldLength = (32 - Cache.setNumFieldLength - Cache.blockOffsetFieldLength);
}

//Outputs the number of bits in the set index  field of theaddress
int setIndexLength(int k, int l, int c){
	int setLength = logBaseTwo(c/(l*k));
	int offsetSize = logBaseTwo(k);
	assert((32 - setLength - offsetSize) > 0);
	return setLength;
}

int setIndexLengthTest(){
	return 0;
}

//Outputs  the  number  of  bits  in  the  line  o sbbet field  of  the address
int offsetLength(int k, int l, int c){
	int setLength = logBaseTwo(c/(l*k));
	int offsetSize = logBaseTwo(k);
	assert((32 - setLength - offsetSize) > 0);
	return offsetSize; 
}

int offsetLengthTest(){
	int length1 = offsetLength(512, 8, 16);
	assert(length1 == 9);
	return 0;
}

//Outputs the tag bits associated with the address


int tagBits(unsigned int address, int offset, int setIndex){	
	assert((offset + setIndex) < 32); 
	address = address >> (offset + setIndex);
	return address;
}

int tagBitsTest(){
	return 0;
}

// If there is a hit, this outputs the cache way in which the accessed line can be found; 
//it returns -1 if there is a cache miss
int hitWay(){
	return 0;
}

int hitWayTest(){
	return 0;
}

//Updates the tagArray and lruArray upon a hit.  This function is only called on a cache hi
int updateOnHit(){
	return 0;
}

int updateOnHitTest(){
	return 0;
}

// Updates the tagArray and lruArray upon a miss.  This function is only called on a cache miss
int updateOnMiss(){
	return 0;
}

int updateOnMissTest(){
	return 0;
}
 
	//argv[1] = set associativity
	//argv[2] = line size in bytes
	//argv[3] = total cache size in kbytes
	//run file >a.out K L C traceFile
int main(int argc, char *argv[]){
	assert(argv[1]>0);
	assert(argv[2]>0);
	assert(argv[3]>0);
	assert(argv[4]>0);

	int hitRate;int k, l, c;
	k = atoi(argv[1]); l = atoi(argv[2]); c = (*argv[3])-48;

	/*
	struct cache Cache;
	Cache.setNumFieldLength = setIndexLength(k,l,c);
	Cache.blockOffsetFieldLength = offsetLength(k,l,c);
	Cache.tagFieldLength = (32 - Cache.setNumFieldLength - Cache.blockOffsetFieldLength);
	*/


	printf("Start, %d arguements: K:%d, L:%d, C:%d File: %s \n", argc, k, l, c, argv[4]);
	

	hitRate = readTrace(argv[4]);


	printf("Done\n");
	return 0;
}

