#include "bitarray.h"
#include <stdio.h> /*printf*/

int main()
{
	 char str[100];
	 bitarray_t arr = 0xfff0000000000011;
	size_t num_of_rotations = 3;

	printf("%lu\n", BitArraySetOff(arr, 50));
	printf("%lu\n", BitArraySetBit(arr, 2, 0));
	printf("%lu\n", BitArraySetAll(arr));
	printf("%lu\n", BitArrayCountOn(arr));
	printf("%lu\n", BitArrayCountOff(arr));
	printf("%lu\n", BitArrayCountOff(arr));
	
	printf("%s\n", BitArrayToString(BitArrayRotL(arr,  num_of_rotations), str));
	printf("%s\n", BitArrayToString(arr, str));
	printf("%s\n", BitArrayToString(BitArrayFlip(arr, 2), str));
	printf("%lu\n", BitArrayCountOnLut(380));
	printf("**tostr %s\n", BitArrayToString(arr, str));
	printf("**%s\n", BitArrayToString(BitArrayMirror(arr), str));
	printf("**%s\n", BitArrayToString(BitArrayMirrorLut(arr), str));
	printf("**%s\n", BitArrayToString(BitArraySetOff(arr, 55), str));
	printf("%s\n\n", BitArrayToString(BitArrayFlip(arr, 55), str));
	printf("**%s\n\n", BitArrayToString(arr, str));
	printf("%s\n", BitArrayToString(arr, str));
	printf("%s\n\n", BitArrayToString(BitArrayMirror(arr), str));
	
	 /*printf("%s\n", BitArrayToString(arr,str));
	 printf("%d\n", BitArrayGetVal(arr, 4));
	printf("%d\n", BitArraySetOn(arr, 2));
	printf("%d\n", BitArraySetOff(arr, 4)); 
	printf("%s\n", BitArrayToString(arr, str));
	printf("%lu\n", BitArrayMirror(arr));*/

	return 0;
}
