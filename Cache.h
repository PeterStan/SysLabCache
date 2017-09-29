#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


struct cache{
int kSetAss;
int lSetLength;
int cSetSizeBytes;
int tagFieldLength;
int setIndexFieldLength;
int blockOffsetFieldLength;
unsigned int **tagArray; 
int **lruArray;
};

struct cache Cache;

char* getNextAddress();
int whichSet();
int setIndexLength();
int offsetLength();
int tagBits();
int hitWay();
int updateOnHit();
int updateOnMiss();
int readTrace();
int accessCache();
int buildCache();

int accessLRU();
int accessTag();
int getSetIndex();
int getWayIndex();