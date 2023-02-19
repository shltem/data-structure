#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/

#include "circbuff.h"
#define MIN(X,Y) (X>Y? Y : X )

struct circbuff
{
	size_t read_index;
	size_t write_index;
	size_t capasety;
	char *buff;
};
/*approved by tedy*/

circbuff_ptr_t CircBuffCreate(size_t capacity)
{
	circbuff_ptr_t buffer = NULL;
	assert(0 != capacity);
	buffer = (circbuff_ptr_t)malloc(sizeof(struct circbuff)+capacity);
	if(NULL == buffer)
	{
		return NULL;
	}
	
	buffer -> capasety = capacity;
	buffer -> read_index = 0;
	buffer -> write_index = 0;
	buffer -> buff = (char *)buffer + sizeof(struct circbuff);
	return buffer;
}

void CircBuffDestroy(circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	free(buffer);
}

int CircBuffIsEmpty(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return (0 == CircBuffSize(buffer));
}

ssize_t CircBuffRead(circbuff_ptr_t buffer, void *to_write_to, size_t num_of_bytes)
{
	size_t index = 0;
	char *to_write_to_char = NULL;
	
	assert(NULL != buffer);
	assert(NULL != to_write_to);
	
	to_write_to_char = to_write_to;
	num_of_bytes = MIN(num_of_bytes, CircBuffSize(buffer));

	for(index = 0; index < num_of_bytes; ++index)
	{
		*(to_write_to_char+index) = *(buffer -> buff + buffer -> read_index);
		++buffer -> read_index;
		
		if((buffer -> read_index) >= buffer -> capasety)
		{
			buffer -> read_index -= buffer -> capasety;
			buffer -> write_index -= buffer -> capasety; 
		}
	}
		
		
	return num_of_bytes;
}

ssize_t CircBuffWrite(circbuff_ptr_t buffer, const void *to_read_from, size_t num_of_bytes)
{
	int is_overide = 0;
	size_t index = 0;
	size_t num_of_bytes_to_return = num_of_bytes;
	char *runner_buf = buffer->buff;
	size_t write_index_runner = buffer->write_index;
	
	assert(NULL != buffer);
	assert(NULL != to_read_from);
	
	is_overide = (CircBuffFreeSpace(buffer) < num_of_bytes);
	buffer -> write_index += num_of_bytes ;
	
	for (index = 0; index < num_of_bytes; ++index, ++write_index_runner)
	{
		if (write_index_runner == buffer -> capasety)
		{
			write_index_runner -= buffer -> capasety;		
		}
		*(runner_buf + write_index_runner) = 
								*((char *)to_read_from + index);
	}
	
	if(is_overide)
	{
		buffer -> read_index = buffer -> write_index % buffer -> capasety;
		buffer -> write_index = buffer -> read_index + buffer -> capasety;
	}
	if(buffer -> write_index > 2 * buffer -> capasety)
	{
		buffer -> write_index %= buffer -> capasety;
		buffer -> write_index += buffer -> capasety;
	}
	return (num_of_bytes_to_return);
}

size_t CircBuffSize(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return ((buffer -> write_index) - (buffer -> read_index));
}

size_t CircBuffFreeSpace(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return ((buffer -> capasety) - CircBuffSize(buffer));
}

