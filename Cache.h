#include <stdio.h>
#include <assert.h>


struct cache{
int tagFieldLength;
int setNumFieldLength;
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