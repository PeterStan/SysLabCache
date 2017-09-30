#include "Cache.h"
//#include "CacheTest.c"



//takes trace file and returns hit rate

double readTrace(char *file){
	double hits, accesses;
	hits = 0;accesses = 0;
	 
	unsigned int address;
	FILE *fp;
	fp = fopen(file, "r+");

	while( fscanf(fp, "%X", &address) != EOF){
		printf("Address: %d, Tag: %d, Set: %d\t",address, tagBits(address), whichSet(address));
		hits += accessCache(address);
		printf("Hits:%f, Accesses: %f\n", hits, accesses);
		Cache.MRU++;
		accesses++;
	}
	fclose(fp);
	printf("Trace Read\n");
	return 1 - (hits/accesses);
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

//increments Lru value at position if 1, or just returns value if -1
int accessLRUArray(int setIndex, int wayIndex, int t){

	if(t == -1){
		return (*((int *)Cache.lruArray+setIndex*Cache.wSetWay+wayIndex));
	}
	else if(t == 1){
		(*((int *)Cache.lruArray+setIndex*Cache.wSetWay+wayIndex)) = Cache.MRU;
		return (*((int *)Cache.lruArray+setIndex*Cache.wSetWay+wayIndex));
	}
	return 0;
}

int getWayIndex(unsigned int address){
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
int whichSet(unsigned int address){
	int mask = ~(0xFFFFFFFF << Cache.setIndexFieldLength); 
  	address = address >> Cache.blockOffsetFieldLength; 
  	return address & mask;
}

/*int whichSetTest(){
	int address1 = 0b10110011010111010000000000000111;
	printf("%d\n", Cache.setIndexFieldLength);
	assert(Cache.setIndexFieldLength == 16);
	assert(Cache.blockOffsetFieldLength == 3);
	assert(whichSet(address1)==40960);
	return 0;
}*/

//returns 0 or 1 based on wether it is a hit or miss
int accessCache(unsigned int address){
	int way, setIndex, r;

	setIndex = whichSet(address);
	way = hitWay(address);
	printf("Way: %d, Set: %d\n", way, setIndex);

	if(way >= 0){
		printf("Hit\n");
		r = updateOnHit(address);
	}
	else{
		printf("Miss\n");
		r = updateOnMiss(address);
	}
	return r;
}


int buildCache(){
	Cache.setIndexFieldLength = setIndexLength();
	Cache.blockOffsetFieldLength = offsetLength();
	Cache.tagFieldLength = (32 - Cache.setIndexFieldLength - Cache.blockOffsetFieldLength);
	Cache.MRU = 0;
	int i,j;

	Cache.tagArray = (unsigned int **) malloc(Cache.kSetAss*sizeof(unsigned int*));
	for (i=0; i < Cache.kSetAss; i++) *(Cache.tagArray + i) = (unsigned int*) malloc(Cache.wSetWay*sizeof(unsigned int));
	Cache.lruArray = (int **) malloc(Cache.kSetAss*sizeof(unsigned int*));
	for (i=0; i < Cache.kSetAss; i++) *(Cache.lruArray + i) = (unsigned int*) malloc(Cache.wSetWay*sizeof(unsigned int));
	
	for(i = 0; i<Cache.kSetAss; i++){
		for(j = 0; j<Cache.wSetWay; j++){
			*((int *)Cache.lruArray+(i*Cache.wSetWay)+j) = -1;
		}
	}
	printf("Cache Built\n");
}

//Outputs the number of bits in the set index  field of theaddress
int setIndexLength(){
	int setLength = lg(Cache.wSetWay);
	int offsetSize = lg(Cache.kSetAss);
	assert((32 - setLength - offsetSize) > 0);
	return setLength;
}

int setIndexLengthTest(){
	return 0;
}

//Outputs  the  number  of  bits  in  the  line  o sbbet field  of  the address
int offsetLength(){
	int setLength = lg(Cache.wSetWay);
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
int hitWay(unsigned int address){
	int setIndex = whichSet(address);
	int tag = tagBits(address);
	int wayIndex;
	for(wayIndex = 0; wayIndex < Cache.kSetAss; wayIndex++){
		if(accessTagArray(setIndex, wayIndex, -1) == tag) return wayIndex;
	}	
	return -1;
}

int hitWayTest(){
	return 0;
}

//Updates the tagArray and lruArray upon a hit.  This function is only called on a cache hit
int updateOnHit(unsigned int address){
	//update LRU only
	accessLRUArray(whichSet(address), getWayIndex(address), 1);
	printf("Updated Hit at: %d\n", address);
	return 1;
}


// Updates the tagArray and lruArray upon a miss.  This function is only called on a cache miss
int updateOnMiss(unsigned int address){
	int way,set;
	
	printf("Updating Miss at: %X\n", address);


	
	set = whichSet(address);
	way = findLRU(set);

	accessLRUArray(set,way,1);
	accessTagArray(set,way, tagBits(address));

	return 0;
}

//returns way of the least recently used place in the cache
int findLRU(int set){
	int lowest,lWay;
	lowest = 0;lWay = 0;
	printf("SET:%d\n", set);
	int i;
	for(i = 0; i < Cache.wSetWay; i++){

		if(*((int *)Cache.lruArray+(set*Cache.wSetWay)+i) < lowest){
			lWay = i;
			lowest = *((int *)Cache.lruArray+(set*Cache.wSetWay)+i);
			//printf("Lowest: %d\n", lWay);
		}
		printf("Set: %d, L Way: %d, Lowest: %d\n", set, lWay, lowest);
	}

	printf("Found LRU\n");
	return lWay;
	//have set, find least recently used in LRUArray, return way

}

int printLRUArray(){
	int i;
	int j;
	for(i = 0; i<Cache.wSetWay; i++){
		for(j = 0; j<Cache.kSetAss; j++){
			printf("%d\t", (*((int *)Cache.lruArray+i*Cache.kSetAss+j)));
		}
		printf("\n");
	}
	printf("lruArray Printed\n");
	return 0;
}

int printTagArray(){
	int i;
	int j;
	for(i = 0; i<Cache.wSetWay; i++){
		for(j = 0; j<Cache.kSetAss; j++){
			printf("%d\t", (*((int *)Cache.tagArray+i*Cache.kSetAss+j)));
		}
		printf("\n");
	}
	return 0;
}
 
	//argv[1] = set associativity
	//argv[2] = line size in bytes
	//argv[3] = total cache size in kbytes
	//run file >a.out K L C traceFile
int main(int argc, char *argv[]){
	assert(argv[1]>0);assert(argv[2]>0);assert(argv[3]>0);assert(argv[4]>0);

	float hitRate;int k, l, c;
	k = atoi(argv[1]); l = atoi(argv[2]); c = atoi(argv[3]);
	Cache.kSetAss = k;Cache.lSetLength = l;Cache.cSetSizeBytes = c;Cache.wSetWay = (c*1000)/(k*l);

	printf("Start, %d arguements: K:%d, L:%d, C:%d File: %s \n", argc, k, l, c, argv[4]);	
	printf("kSetAss: %d, lSetLength: %d, cSetSizeBytes: %d, wSetWay: %d\n", Cache.kSetAss, Cache.lSetLength, Cache.cSetSizeBytes, Cache.wSetWay);

	buildCache();



	hitRate = readTrace(argv[4]);



	printf("Miss Rate: %f\n", hitRate);
	printf("Done\n");
	return 0;
}

