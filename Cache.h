#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


struct cache{
<<<<<<< HEAD
=======
int kSetAss;
int lSetLength;
int cSetSizeBytes;
int wSetWay;
>>>>>>> c832ca113e43363f976eecde098e91393f867f7d
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