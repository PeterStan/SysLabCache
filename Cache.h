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

int whichSet(); 
int setIndexLength();
int offsetLength();
int tagBits();
int hitWay();
int updateOnHit();
int updateOnMiss();
double readTrace(char *file); 
int accessCache();
int buildCache(); 
int findLRU(int set); 
int lg(int x);

int accessTagArray(int setIndex, int wayIndex, int t); 
int accessLRUArray(int setIndex, int wayIndex, int t);
int getSetIndex();
int getWayIndex();

/*
int getWayIndex(int address);
int whichSet(int address);
int whichSetTest();
int accessCache(int address);
int buildCache(); //done, not tested
int setIndexLength();
int offsetLength();
int offsetLengthTest();
int tagBits(unsigned int address);
int tagBitsTest();
int hitWay(int address);
int hitWayTest();
int updateOnHit(int address);
int updateOnHitTest();
int updateOnMiss(int address);

int updateOnMissTest();
*/
























