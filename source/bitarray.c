#include "../include/bitarray.h"
#include <stdio.h> /*printf*/
#

#define NUMBERS_OF_BITS_IN_ARRAY (64)
#define WORD_SIZE (8)
#define WORD_SIZE_IN_64_BITS (8)
#define ALL_BITS_SET_IN_BYTE (256)
static void InitMirrorLut();
static void InitCountLut(size_t *lut_array);
static bitarray_t BitArrayMirrorWordSize(bitarray_t arr);
static size_t lut_array_mirr[256];
/*approved by shay */
static void InitMirrorLut()
{
	size_t index = 0;
	for (index = 0; index < ALL_BITS_SET_IN_BYTE ; ++index)
	{
		lut_array_mirr[index] = BitArrayMirrorWordSize(index);

	}							
}


static void InitCountLut(size_t *lut_array)
{
	size_t index = 0;
	for (index = 0; index < ALL_BITS_SET_IN_BYTE ; ++index)
	{
		lut_array[index] = BitArrayCountOn(index);
	}
							
}

bitarray_t BitArraySetAll(bitarray_t arr)
{
	arr = 0;
	arr = ~arr;
	
	return arr;
}

bitarray_t BitArraySetOn(bitarray_t arr, size_t index)
{
	bitarray_t num_1 = 1;
	
	arr = arr ^ (num_1 << index);
	
	return (arr);
	
	
}

int BitArrayGetVal(bitarray_t arr, size_t index)
{
	bitarray_t num_1 = 1;
	return ( 0 != ((arr) & (num_1 << (index))));
	
}

bitarray_t BitArrayRotR(bitarray_t arr, size_t num_of_rotations)
{
	bitarray_t temp_operation = arr;

	temp_operation <<= 64 - num_of_rotations;
	arr >>= num_of_rotations;
	arr |= temp_operation;

	return arr;
}

size_t BitArrayCountOn(bitarray_t arr)
{
	
	size_t index = 0;
	size_t counter = 0;
	
	for (index = 0; index < NUMBERS_OF_BITS_IN_ARRAY; ++index)
	{
		if (arr & 1)
		{
			++counter;
		}
		arr = (arr >> 1);
	}
	
	return counter;
}

bitarray_t BitArrayResetAll(bitarray_t arr)
{
	arr = 0;
	return arr;
}

bitarray_t BitArraySetOff(bitarray_t arr, size_t index)
{
	bitarray_t num_1 = 1;
	
	arr = arr & (~(num_1 << index));
	
	return (arr);
}

bitarray_t BitArrayFlip(bitarray_t arr, size_t index)
{
		bitarray_t num_1 = 1;
 		arr ^= (num_1 << index) ;

	return arr;
}

bitarray_t BitArrayRotL(bitarray_t arr, size_t num_of_rotations)
{
	bitarray_t temp_operation = arr;

	temp_operation >>= 64 - num_of_rotations;
	arr <<= num_of_rotations;
	arr |= temp_operation;
	
	return arr;
}

size_t BitArrayCountOff(bitarray_t arr)
{
	size_t index = 0;
	size_t counter = 0;
	
	for (index = 0; index < NUMBERS_OF_BITS_IN_ARRAY; ++index)
	{
		if (0 == (arr & 1))
		{
			++counter;
		}
		arr = (arr >> 1);
	}
	
	return counter;
}

char *BitArrayToString(bitarray_t arr, char *to_write_into)
{
	size_t index = 0;
	char set = '1';
	char off = '0';
	bitarray_t num_1 = 1;
	
	for (index = 0; index < NUMBERS_OF_BITS_IN_ARRAY; ++index)
	{
		if (0 != (arr & num_1))
		{
			to_write_into[NUMBERS_OF_BITS_IN_ARRAY-1-index] = set;
		}
		else
		{
			to_write_into[NUMBERS_OF_BITS_IN_ARRAY-1-index] = off;
		}
		num_1 <<= 1;
	}
	
	to_write_into[64] = '\0';
	
	return to_write_into;
}

bitarray_t BitArraySetBit(bitarray_t arr, size_t index, int value)
{

	if (1 == value)
	{
		arr = BitArraySetOn(arr, index);
	}
	else
	{
		arr = BitArraySetOff(arr, index);
	}
	
	return (arr);
	
	
}

bitarray_t BitArrayMirror(bitarray_t arr)
{
	bitarray_t temp_bit = 0;
	size_t index = 0;
	bitarray_t miror_arr = 0;
	 
	for(index = 0; index < NUMBERS_OF_BITS_IN_ARRAY ; ++index)
	{
		temp_bit = arr & 1;
		miror_arr = miror_arr|temp_bit;
		arr = arr >> 1;
		if (index < (NUMBERS_OF_BITS_IN_ARRAY-1))
		{
			miror_arr = miror_arr << 1;
		}
	}
	
	return miror_arr;
}

bitarray_t BitArrayMirrorWordSize(bitarray_t arr)
{
	bitarray_t temp_bit = 0;
	size_t index = 0;
	bitarray_t miror_arr = 0;
	 
	for(index = 0; index < WORD_SIZE ; ++index)
	{
		temp_bit = arr & 1;
		miror_arr = miror_arr|temp_bit;
		arr = arr >> 1;
		if (index < (WORD_SIZE-1))
		{
			miror_arr = miror_arr << 1;
		}
	}
	
	return miror_arr;
}

bitarray_t BitArrayCountOnLut(bitarray_t arr)
{
	static size_t lut_array_count[256] = {0};
	size_t index = 0;
	size_t counter = 0;
	
	InitCountLut(lut_array_count);
	
	for(index = 0; index < WORD_SIZE_IN_64_BITS ; ++index)
	{
		counter += lut_array_count[(arr & ALL_BITS_SET_IN_BYTE)];
		arr >>= WORD_SIZE;
	}
	
	return counter;
}

bitarray_t BitArrayMirrorLut(bitarray_t arr)
{
	size_t index = 0;
	bitarray_t miror_arr = 0;
	
	InitMirrorLut();
	 
	for(index = 0; index < WORD_SIZE ; ++index)
	{
		miror_arr |= (lut_array_mirr[(arr & ALL_BITS_SET_IN_BYTE)] << (WORD_SIZE-1));
		arr >>= WORD_SIZE;
	}
	
	return miror_arr;
}
