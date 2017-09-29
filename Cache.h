#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


struct cache{
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
int readTrace();
int accessCache();
int buildCache();

int accessTagArray(int setIndex, int wayIndex, int t); //done, kinda tested
int accessLRUArray(int setIndex, int wayIndex, int t); //done, kinda tested
int getSetIndex();
int getWayIndex();

/*
int readTrace(char *file); //done and tested
int accessTagArray(int setIndex, int wayIndex, int t); //done, kinda tested
int accessLRUArray(int setIndex, int wayIndex, int t); //done, kinda tested
int getWayIndex(int address);
int lg(int x);  //done
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
int findLRU(int address);
int updateOnMissTest();
*/