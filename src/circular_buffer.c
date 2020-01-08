#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

typedef uint8_t bool;

void initCircularBuffer(struct circularBuffer* bufferPtr, int *data, int maxLength) 
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->size = 0;
  bufferPtr->maxLength = maxLength;
}

int addElementCircularBuffer(struct circularBuffer *bufferPtr, int value)
{
	// if(bufferPtr->size >= bufferPtr->maxLength)
	// {
	// 	 assert(0 && "Size grew out of bounds!");
	// 	return INT_MIN;
	// }
  	if(bufferPtr->tail + 1 >= bufferPtr->maxLength){
		return INT_MIN;
  	}
  	else if (containsCircularBuffer(bufferPtr, value) == value){
		return INT_MIN;
	}else{
		bufferPtr->data[bufferPtr->tail] = value;
		bufferPtr->size++;
		bufferPtr->tail++;
	}

	return value;
}

int reset_buffer(struct circularBuffer* bufferPtr)
{
	bufferPtr->size = 0;
	bufferPtr->data = NULL;
	bufferPtr->tail = 0;
	bufferPtr->head = 0;
	bufferPtr->maxLength = 0;
}

int find_index(struct circularBuffer* bufferPtr, int value)
{
	for(int i = 0; i < bufferPtr->size; i++)
	{
		if(bufferPtr->data[i] == value)
			return value;
	}
	return INT_MIN;
}

int rfind_index(struct circularBuffer* bufferPtr, int value)
{
	for(int i = bufferPtr->maxLength - 1; i >= 0; i--)
	{
		if(bufferPtr->data[i] == value)
			return value;
	}
	return INT_MIN;
}

int removeValueCircularBuffer(struct circularBuffer* bufferPtr, int value)
{
	bool found = FALSE;
	while(1)
	{
		const int index = rfind_index(bufferPtr,value);

		if(found == TRUE && index < 0)
			return value;
		else if(index < 0)
			return INT_MIN;
		
		for(int j = index; j >= 0; j--)
		{
			bufferPtr->data[j] = bufferPtr->data[j-1];
		}
		bufferPtr->head++;

		if(bufferPtr->head >= bufferPtr->maxLength)
			bufferPtr->head = 0;

		bufferPtr->size--;
		found = TRUE;
	}
	return INT_MIN;
}

int getElementCircularBuffer(struct circularBuffer* bufferPtr)
{
	int return_value = bufferPtr->data[bufferPtr->head];
	bufferPtr->data[bufferPtr->head] = INT_MIN;
	bufferPtr->head++;
	
	if(bufferPtr->head >= bufferPtr->maxLength)
			bufferPtr->head = 0;
	return return_value;
}

int containsCircularBuffer(struct circularBuffer* bufferPtr, int value)
{
	for(int i = 0; i < bufferPtr->size; i++)
	{
		if(bufferPtr->data[i] == value)
			return value;
	}

	return INT_MIN;
}

int print_buffer(struct circularBuffer* bufferPtr)
{
	for(int i = 0; i < bufferPtr->maxLength; i++)
	{
		printf("\n%d", bufferPtr->data[i]);
	}
	printf("\n");
	return 0;
}