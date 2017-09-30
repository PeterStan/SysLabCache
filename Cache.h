#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


struct cache{
int MRU;
int kSetAss;
int lSetLength;
int cSetSizeBytes;
int wSetWay;
int tagFieldLength;
int setIndexFieldLength;
int blockOffsetFieldLength;
unsigned int **tagArray; 
int **lruArray;
};

struct cache Cache;

int whichSet(unsigned int address);
int setIndexLength();
int offsetLength();
int tagBits(unsigned int address);
int hitWay(unsigned int address);
int updateOnHit(unsigned int address, int way);
int updateOnMiss(unsigned int address);
double readTrace(char *file);
int accessTagArray(int setIndex, int wayIndex, int t);
int accessLRUArray(int setIndex, int wayIndex, int t);
int lg(int x);
int accessCache(unsigned int address);
int buildCache();
int findLRU(int set);


