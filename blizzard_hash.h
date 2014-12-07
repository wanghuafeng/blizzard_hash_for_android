#ifndef __BLIZZARD_HASH_H
#define __BLIZZARD_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define EXPORT

#pragma pack(1)
typedef struct{
    unsigned nHashA:7;
	unsigned bExists:1;
}MPQHASHTABLE;

EXPORT unsigned long cryptTable[0x500];

EXPORT unsigned long HashString(char *lpszString, unsigned long dwHashType);
EXPORT char* MPQHashTableInit(char **ppHashTable, long nTableLength);
EXPORT void MPQHashTableFree(char *pHashTable);
EXPORT unsigned int MPQHashTableAdd(char *lpszString, char *pHashTable);
EXPORT long MPQHashTableIsExist(char *lpszString, char *pHashTable);

#endif
