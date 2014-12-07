#include<stdio.h>
#include<string.h>
#pragma pack(1)
typedef struct{
    int nHashA:7;
	int bExists:1;
}MPQHASHTABLE;
#pragma pack()

void bitOperation()
{
    char a = 'a';
    printf("a&0b11111110 is: %d\n", a&0b11111110);
    printf("%d\n", a);
    printf("0b11111110 convert int is: %d\n", 0b11111110);
}
int hashTable()
{
    //printf("length of MPQHASHTABLE is: %d\n", sizeof(MPQHASHTABLE));
    char *p = NULL;
    MPQHASHTABLE *_pHashTable = NULL;
	p = malloc(sizeof(MPQHASHTABLE));
    _pHashTable = (MPQHASHTABLE *)p;
    _pHashTable->nHashA = 1;
    _pHashTable->bExists = 0b0;
    printf("_pHashTable->bExists is: %d\n", _pHashTable->bExists);
    _pHashTable->bExists = 0b1;
    printf("_pHashTable->bExists: %d\n", _pHashTable->bExists);//*/
}
bitArea()
{
    #pragma pack(1)
    struct bs
    {
        unsigned a:1;
        //unsigned b:3;
        unsigned c:7;
    } bit,*pbit;

    printf("lenght of struct bs is : %d\n", sizeof(struct bs));
    bit.a=1;
    //bit.b=7;
    bit.c=255;
    printf("a:%d,c:%d\n",bit.a,bit.c);
    bit.a = 0;
    long b = 255;
    printf("b get low 7 bit is : %d\n", bit.c == (b&0b1111111));/*注意此处 & 的优先级低于 == 所以后面一定要加上括号*/
    printf("change bit.a is :%d\n", bit.a);
}
void test_sprintf()
{
    char str[20];
    int num = 234;
    sprintf(str, "%d", num);
    //printf("%s", str);
}
void main()
{
    //printf("length of MPQHASHTABLE is: %d\n", sizeof(MPQHASHTABLE));
    //bitOperation();
    //bitArea();
    test_sprintf();
}
