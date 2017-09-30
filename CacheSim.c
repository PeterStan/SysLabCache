#include "Cache.h"
//#include "CacheTest.c"

//Outputs the cache set in which the address falls
int whichSet(unsigned int address){
	int mask = ~(0xFFFFFFFF << Cache.setIndexFieldLength); //generate mask for unwanted tag bits
  	address = address >> Cache.blockOffsetFieldLength; //shift to eliminate offset bits
  	return address & mask;
}

//Outputs the number of bits in the set index  field of theaddress
int setIndexLength(){
	int setLength = lg(Cache.wSetWay);//logbase2 of # of sets
	int offsetSize = lg(Cache.kSetAss);//logbase2 of # of ways per set
	assert((32 - setLength - offsetSize) > 0);//make sure the index and offset sizes dont exceed 32
	return setLength;
}

//Outputs  the  number  of  bits  in  the  line  o sbbet field  of  the address
int offsetLength(){
	//same as setIndexLength() but output the offset instead
	int setLength = lg(Cache.wSetWay);
	int offsetSize = lg(Cache.kSetAss);
	assert((32 - setLength - offsetSize) > 0);
	return offsetSize; 
}

//Outputs the tag bits associated with the address
int tagBits(unsigned int address){	
	int shift = Cache.blockOffsetFieldLength + Cache.setIndexFieldLength;
	assert(shift < 32); 
	address = address >> (shift); //shift right by setindex and offset bits to retrieve the tag
	return address;
}

// If there is a hit, this outputs the cache way in which the accessed line can be found; 
//it returns -1 if there is a cache miss
int hitWay(unsigned int address){
	int setIndex = whichSet(address);
	int tag = tagBits(address);
	int wayIndex;
	for(wayIndex = 0; wayIndex < Cache.kSetAss; wayIndex++){
		if(accessTagArray(setIndex, wayIndex, -1) == tag) return wayIndex;
	}	//loop checks each way in a set, if the tag matches, returns the way
	return -1;
}

//Updates the tagArray and lruArray upon a hit.  This function is only called on a cache hit
int updateOnHit(unsigned int address, int way){
	accessLRUArray(whichSet(address), way, 1);
	return 1;
}

// Updates the tagArray and lruArray upon a miss.  This function is only called on a cache miss
int updateOnMiss(unsigned int address){
	int way,set;
	
	set = whichSet(address);
	way = findLRU(set);

	accessLRUArray(set,way,1);
	accessTagArray(set,way, tagBits(address));

	return 0;
}

//takes trace file and returns hit rate
//called by main()
double readTrace(char *file){
	double hits, accesses;
	hits = 0;accesses = 0;
	 
	unsigned int address;
	FILE *fp;
	fp = fopen(file, "r+");

	while( fscanf(fp, "%X", &address) != EOF){
		hits += accessCache(address);
		Cache.MRU++;accesses++;
	}
	fclose(fp);
	return 1 - (hits/accesses);
}

//sets tag in array to t, or if t is -1 return tag at that index
//called by hitWay() and updateOnMiss()
int accessTagArray(int setIndex, int wayIndex, int t){
	assert(setIndex<=Cache.wSetWay);
	assert(wayIndex<=Cache.kSetAss);

	if(t == -1){
		t = (*((int *)Cache.tagArray+setIndex*Cache.kSetAss+wayIndex));
	}
	else{
		*((int *)Cache.tagArray+(setIndex*Cache.kSetAss)+wayIndex) = t;
	}
	return t;
}

//increments Lru value at position if 1, or just returns value if -1
//called by updateOnMiss() and updateOnHit()
int accessLRUArray(int setIndex, int wayIndex, int t){

	if(t == -1){
		return (*((int *)Cache.lruArray+setIndex*Cache.kSetAss+wayIndex));
	}
	else if(t == 1){
		(*((int *)Cache.lruArray+setIndex*Cache.kSetAss+wayIndex)) = Cache.MRU;
		return (*((int *)Cache.lruArray+setIndex*Cache.kSetAss+wayIndex));
	}
	return 0;
}

//returns log base 2 of x, rounding up 
//called by setIndexLenth() and offsetLength()
int lg(int x){

	int i;
	for(i = 1; (1<<i) <= x; i++)
		if(x == (1<<i))return i;
	return i;
}

//returns 0 or 1 based on wether it is a hit or miss
//called by readTrace(), readTrace() is called by main()
int accessCache(unsigned int address){
	int way, setIndex, r;

	setIndex = whichSet(address);
	way = hitWay(address);

	if(way >= 0){
		r = updateOnHit(address, way);
	}
	else{
		r = updateOnMiss(address);
	}
	return r;
}

//called by main()
int buildCache(){
	Cache.setIndexFieldLength = setIndexLength();
	Cache.blockOffsetFieldLength = offsetLength();
	Cache.tagFieldLength = (32 - Cache.setIndexFieldLength - Cache.blockOffsetFieldLength);
	Cache.MRU = 0;
	int i,j;

	//allocate space for LRUArray and TagArray
	Cache.tagArray = (unsigned int **) malloc(Cache.wSetWay*sizeof(unsigned int*));
	for (i=0; i < Cache.wSetWay; i++){
		*(Cache.tagArray + i) = (unsigned int*) malloc(Cache.kSetAss*sizeof(unsigned int));
	}
	Cache.lruArray = (int **) malloc(Cache.wSetWay*sizeof(unsigned int*));
	for (i=0; i < Cache.wSetWay; i++){ 
		*(Cache.lruArray + i) = (unsigned int*) malloc(Cache.kSetAss*sizeof(unsigned int));
	}

	//intializing all values in the LRU array to -1
	for(i = 0; i<Cache.wSetWay; i++){
		for(j = 0; j<Cache.kSetAss; j++){
			*((int *)Cache.lruArray+(i*Cache.kSetAss)+j) = -1;
		}
	}
}


//returns way of the least recently used place in the cache
//have set, find least recently used in LRUArray, return way
//called by updateOnMiss()
int findLRU(int set){
	int lowest,lWay;
	lowest = 0;lWay = 0;

	int i;
	for(i = 0; i < Cache.kSetAss; i++){

		if(*((int *)Cache.lruArray+(set*Cache.kSetAss)+i) < lowest){
			lWay = i;
			lowest = *((int *)Cache.lruArray+(set*Cache.kSetAss)+i);
		}
	}

	return lWay;
}
 
	//argv[1] = set associativity
	//argv[2] = line size in bytes
	//argv[3] = total cache size in kbytes
	//run file >a.out K L C traceFile
int main(int argc, char *argv[]){
	assert(argv[1]>0);assert(argv[2]>0);assert(argv[3]>0);assert(argv[4]>0);

	float missRate;int k, l, c;
	k = atoi(argv[1]); l = atoi(argv[2]); c = atoi(argv[3]);
	Cache.kSetAss = k;Cache.lSetLength = l;Cache.cSetSizeBytes = c;Cache.wSetWay = (c*1000)/(k*l);


	buildCache();
	missRate = readTrace(argv[4]);

	printf("%s %d %d %d %.2f\n",argv[4], c, k, l, missRate);
	return 0;
}

