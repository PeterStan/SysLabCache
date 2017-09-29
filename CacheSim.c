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
	while((quantity-(2^x))>0){
		x++;
	}
	return x;
	free(x);
	free(quantity);
}

int lg(int x){//returns log base 2 of x, or -1 
	int i;
	for(i = 1; (1<<i) <= x; i++)
		if(x == (1<<i))return i;
	return i;
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
	int way, setIndex, tag, r;

	setIndex = 0;
	tag = tagBits(address);
	way = hitWay(address);

	if(way >= 0){
		r = updateOnHit();
	}
	else{
		r = updateOnMiss();
	}
	return r;
}

	//argv[1] = set associativity
	//argv[2] = line size in bytes
	//argv[3] = total cache size in kbytes
int buildCache(int k, int l, int c){
	Cache.setNumFieldLength = setIndexLength(k,l,c);
	Cache.blockOffsetFieldLength = offsetLength(k,l,c);
	Cache.tagFieldLength = (32 - Cache.setNumFieldLength - Cache.blockOffsetFieldLength);

	tagArray = (unsigned int **) malloc(k*sizeof(unsigned int*));
	*tagArray = (unsigned int*) malloc((c/(k*l))*sizeof(unsigned int));
	lruArray = (int **) malloc(k*sizeof(int*));
	*lruArray = (int*) malloc((c/(k*l))*sizeof(int));


}

//Outputs the number of bits in the set index  field of theaddress
int setIndexLength(int k, int l, int c){
	int setLength = lg(c/(l*k));
	int offsetSize = lg(k);
	assert((32 - setLength - offsetSize) > 0);
	return setLength;
}

int setIndexLengthTest(){
	return 0;
}

//Outputs  the  number  of  bits  in  the  line  o sbbet field  of  the address
int offsetLength(int k, int l, int c){
	//int setLength = logBaseTwo(c/(l*k));
	int offsetSize = lg(k);
	//assert((32 - setLength - offsetSize) > 0);
	return offsetSize; 
}

int offsetLengthTest(){
	int length1 = offsetLength(512, 8, 16);
	assert(length1 == 9);
	return 0;
}

//Outputs the tag bits associated with the address
int tagBits(unsigned int address){	
	int shift = Cache.blockOffsetFieldLength + Cache.setIndexFieldLength;
	assert(shift < 32); 
	address = address >> (shift);
	return address;
}

int tagBitsTest(){
	return 0;
}

// If there is a hit, this outputs the cache way in which the accessed line can be found; 
//it returns -1 if there is a cache miss
int hitWay(int address){
	return 0;
}

int hitWayTest(){
	return 0;
}

//Updates the tagArray and lruArray upon a hit.  This function is only called on a cache hit
int updateOnHit(int address){
	//update LRU

	return 1;
}

int updateOnHitTest(){
	return 1;
}

// Updates the tagArray and lruArray upon a miss.  This function is only called on a cache miss
int updateOnMiss(int address){
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
	k = atoi(argv[1]); l = atoi(argv[2]); c = ((*argv[3])-48)*8000;


	printf("Start, %d arguements: K:%d, L:%d, C:%d File: %s \n", argc, k, l, c, argv[4]);	

	hitRate = readTrace(argv[4]);



	printf("Done\n");
	return 0;
}

