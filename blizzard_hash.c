#include <stdio.h>
#include<assert.h>
#include<stdlib.h>
#include "blizzard_hash.h"

static unsigned int g_hash_mask = 0;

static void InitCryptTable()
{
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for (index1 = 0; index1 < 0x100; index1++)
	{
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = (temp1 | temp2);
		}
	}
}

EXPORT unsigned long HashString(char *lpszString, unsigned long dwHashType)
{
	unsigned char *key = (unsigned char *)lpszString;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;

	while(*key != 0)
	{
		ch = toupper(*key++);

		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

EXPORT char* MPQHashTableInit(char **ppHashTable, long nTableLength)
{
	long i = 0;
	char *p = NULL;
	MPQHASHTABLE *_pHashTable = NULL;

	/* 检查nTableLength是否为2的幂次方 */
	//assert(nTableLength&(nTableLength-1)==0);

	g_hash_mask = nTableLength - 1;

	InitCryptTable();

	p = malloc(nTableLength * sizeof(MPQHASHTABLE));

	if (p == NULL)
	{
		printf("%s, %d: malloc failed!\n", __FUNCTION__, __LINE__);
		return p;
	}
	*ppHashTable = p;
	_pHashTable = (MPQHASHTABLE *)p;

	for (i = 0; i < nTableLength; i++)
	{
		(_pHashTable + i)->nHashA = 0;
		(_pHashTable + i)->bExists = 0;
	}

	return p;
}

EXPORT void MPQHashTableFree(char *pHashTable)
{
	if (pHashTable != NULL)
	{
		free(pHashTable);
		pHashTable = NULL;
	}
}

EXPORT unsigned int MPQHashTableAdd(char *lpszString, char *pHashTable)
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	//__int8 nHashA = (__int8)HashString(lpszString, HASH_A);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	unsigned long nHashStart = nHash & g_hash_mask;
	unsigned long nHashPos = nHashStart;
	MPQHASHTABLE *_pHashTable = (MPQHASHTABLE *)pHashTable;

	while ((_pHashTable + nHashPos)->bExists)
	{
		nHashPos = (nHashPos + 1)  & g_hash_mask;

		if (nHashPos == nHashStart)
		{
			return 0;
		}
	}
	(_pHashTable + nHashPos)->nHashA = nHashA;

	//printf("nHashA is : %d\n",(_pHashTable + nHashPos)->nHashA);

	(_pHashTable + nHashPos)->bExists = 1;

    //printf("(_pHashTable + nHashPos)->bExists: %d\n", (_pHashTable + nHashPos)->bExists);
	return 1;
}

EXPORT long MPQHashTableIsExist(char *lpszString, char *pHashTable)
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	//__int8 nHashA = (__int8)HashString(lpszString, HASH_A);
	unsigned long nHashStart = nHash & g_hash_mask;
	unsigned long nHashPos = nHashStart;
	MPQHASHTABLE *_pHashTable = (MPQHASHTABLE *)pHashTable;

	//printf("(_pHashTable + nHashPos)->bExists is: %d\n", (_pHashTable + nHashPos)->bExists);


	while ((_pHashTable + nHashPos)->bExists)
	{
		/*printf("hash string nHashA is: %d\n", nHashA);
        printf("low 7 bit nHashA is : %d\n", nHashA&0b1111111);

		printf("(_pHashTable + nHashPos)->nHashA is: %d\n", (_pHashTable + nHashPos)->nHashA == (nHashA&0b1111111));*/
		if ((_pHashTable + nHashPos)->nHashA == (nHashA&0b1111111))
		{
			return 1;
		}
		else
		{
			nHashPos = (nHashPos + 1) & g_hash_mask;
		}
		if (nHashPos == nHashStart)
		{
			break;
		}
	}
	return -1;
}

void test()
{
	char *p = NULL;
	char *strs[] = {
		"t1", "哈希", "t3"
	};
	long IsExits;
	int i;

	char *HashTablePos = MPQHashTableInit(&p, 1000000);

	//printf("pos of hashtable is %d\n", HashTablePos);

	for (i = 0; i < sizeof(strs) / sizeof(char*); ++i)
	{
		MPQHashTableAdd(strs[i], HashTablePos);
	}

	IsExits = MPQHashTableIsExist("t3", HashTablePos);
	if (IsExits == -1)
	{
	printf("failed...\n");
	}
	else
	printf("sucess ...\n");

}

void add_test()
{
	char *p = NULL;
	int i;
	int j;
	int crash_count = 0;
	int failed_count = 0;
	char *HashTablePos = MPQHashTableInit(&p, 2097152);
	unsigned int add_sucess;
	unsigned isExists;
	//printf("HashTablePos is : %d\n", HashTablePos);
	for (i = 0; i < 2000000; i++)
	{
		char str[20];
		sprintf(str, "%d", i);
		add_sucess = MPQHashTableAdd(str, HashTablePos);
		if (add_sucess == 0)
		{
			crash_count += 1;
		}
	}
	printf("crash_count : %d\n", crash_count);

	//check if all string already in hashtable...
	for (j = 0 ; j < 2000000; j++)
	{
        char str[10];
        sprintf(str, "%d", j);
        isExists = MPQHashTableIsExist(str, HashTablePos);
        if (isExists == -1)
        {
            failed_count ++;
        }
	}
	printf("failed count is %d\n", failed_count);
}

int main()
{
	//test();
	add_test();
}
