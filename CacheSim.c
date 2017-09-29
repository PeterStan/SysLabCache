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

//sets tag in array to t, or if t is -1 return tag at that index
int accessTagArray(int setIndex, int wayIndex, int t){

	if(t == -1){
		t = (*((int *)Cache.tagArray+setIndex*Cache.wSetWay+wayIndex));
	}
	else{
		*((int *)Cache.tagArray+(setIndex*Cache.wSetWay)+wayIndex) = t;
	}
	return t;
}

int accessLRUArray(int setIndex, int wayIndex){
	return 0;
}

int getWayIndex(int address){
	int mask = ~(0xFFFFFFFF << Cache.blockOffsetFieldLength);
	return address & mask;
}

int lg(int x){//returns log base 2 of x, or -1 
	int i;
	for(i = 1; (1<<i) <= x; i++)
		if(x == (1<<i))return i;
	return i;
}

//Outputs the cache set in which the address falls
int whichSet(int address){
	int mask = ~(0xFFFFFFFF << Cache.setIndexFieldLength); 
  	address = ((unsigned int)address) >> Cache.blockOffsetFieldLength; 
  	return address & mask;
}

int whichSetTest(){
	//assert(whichSet() == 0);
	return 0;
};

//returns 0 or 1 based on wether it is a hit or miss
int accessCache(int address){
	int way, setIndex, r;

	setIndex = getSetIndex(address);
	way = hitWay(address);

	if(way >= 0){
		r = updateOnHit();
	}
	else{
		r = updateOnMiss();
	}
	return r;
}


int buildCache(){
	Cache.setIndexFieldLength = setIndexLength();
	Cache.blockOffsetFieldLength = offsetLength();
	Cache.tagFieldLength = (32 - Cache.setIndexFieldLength - Cache.blockOffsetFieldLength);

	Cache.tagArray = (unsigned int **) malloc(Cache.kSetAss*sizeof(unsigned int*));
	for (int i=0; i < Cache.kSetAss; i++) *(Cache.tagArray + i) = (unsigned int*) malloc(Cache.wSetWay*sizeof(unsigned int));
	Cache.lruArray = (int **) malloc(Cache.kSetAss*sizeof(unsigned int*));
	for (int i=0; i < Cache.kSetAss; i++) *(Cache.lruArray + i) = (unsigned int*) malloc(Cache.wSetWay*sizeof(unsigned int));
	

	for(int i = 0; i<Cache.kSetAss; i++){
		for(int j = 0; j<Cache.wSetWay; j++){
			*((int *)Cache.lruArray+(i*Cache.wSetWay)+j) = -1;
		}
	}

	//intialize lru array, all values in lruArray to -1
}

//Outputs the number of bits in the set index  field of theaddress
int setIndexLength(){
	int setLength = lg((Cache.cSetSizeBytes*8000)/(Cache.wSetWay));
	int offsetSize = lg(Cache.kSetAss);
	assert((32 - setLength - offsetSize) > 0);
	return setLength;
}

int setIndexLengthTest(){
	return 0;
}

//Outputs  the  number  of  bits  in  the  line  o sbbet field  of  the address
int offsetLength(){
	int setLength = lg((Cache.cSetSizeBytes*8000)/(Cache.wSetWay));
	int offsetSize = lg(Cache.kSetAss);
	assert((32 - setLength - offsetSize) > 0);
	return offsetSize; 
}

int offsetLengthTest(){
	int length1 = offsetLength(512, 8, 16);
	//assert(length1 == 9);
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
	int setIndex = whichSet(address);
	int tag = tagBits(address);
	for(int wayIndex = 0; wayIndex <= Cache.kSetAss; wayIndex++){
		if(accessTagArray(setIndex, wayIndex, -1) == tag) return wayIndex;
	}	
	return -1;
}

int hitWayTest(){
	return 0;
}

//Updates the tagArray and lruArray upon a hit.  This function is only called on a cache hit
int updateOnHit(int address){
	//update LRU only

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
	assert(argv[1]>0);assert(argv[2]>0);assert(argv[3]>0);assert(argv[4]>0);

	int hitRate;int k, l, c;
	k = atoi(argv[1]); l = atoi(argv[2]); c = ((*argv[3])-48);
	Cache.kSetAss = k;Cache.lSetLength = l;Cache.cSetSizeBytes = c;Cache.wSetWay = c/(k*l);

	printf("Start, %d arguements: K:%d, L:%d, C:%d File: %s \n", argc, k, l, c, argv[4]);	


	buildCache();
	hitRate = readTrace(argv[4]);

	printf("Done\n");
	return 0;
}

