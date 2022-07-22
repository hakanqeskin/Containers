/*
 * array.c
 *
 *  Created on: Jan 9, 2019
 *      Author: hakan.keskin
 */

#include "include/containers/array.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

struct Array{
	size_t itemSize;
	size_t count;
	FDestructor destructor;

	size_t capacity;
	unsigned char *data;
};

static int array_setCapacity(array_t self, size_t capacity)
{
	void *newData = realloc(self->data, self->itemSize * capacity );
	if( !newData ){
		return -1;
	}

	self->capacity = capacity;
	self->data = newData;
	return 0;
}

static int array_checkCapacity(array_t self)
{
	int result = 0;
	if( self->capacity == self->count ){
		size_t new_capacity = self->count ? self->capacity << 1 : 1;
		result = array_setCapacity(self, new_capacity);
	}

	return result;
}

array_t array_create(size_t itemSize, FDestructor destructor)
{
	array_t self;
	if( itemSize ){
		if( (self = malloc(sizeof(struct Array))) != NULL ){
			self->destructor = destructor;
			self->itemSize = itemSize;
			self->count = 0;
			self->capacity = 0;
			self->data = NULL;
		}
	}
	else{
		errno = EINVAL;
		self = NULL;
	}

	return self;
}

static void array_reset(array_t self)
{
	if( self->destructor ){
		size_t I;
		for(I = 0; I < self->count; ++I){
			self->destructor(self->data + I * self->itemSize);
		}
	}
}

void array_delete(array_t self)
{
	if( self ){
		array_reset(self);
		free(self->data);
	}
	free(self);
}

void array_clear(array_t self)
{
	assert(self);
	array_reset(self);
	self->count = 0;
}

size_t array_size(const array_t self)
{
	assert(self);
	return self->count;
}

void array_resize(array_t self, size_t count)
{
	assert(self);
	if( count > self->count ){
		if( count > self->capacity ){
			if( array_setCapacity(self, count) == -1 ){
				return;
			}
		}
	}
	else if( count < self->count){
		if( self->destructor ){
			size_t I;
			for(I = count; I < self->count; ++I){
				self->destructor(self->data + I * self->itemSize);
			}
		}
	}

	self->count = count;
}

size_t array_capacity(const array_t self)
{
	assert(self);
	return self->capacity;
}

void array_reserve(array_t self, size_t cap)
{
	assert(self);
	if( cap > self->capacity ){
		array_setCapacity(self, cap);
	}
}

void array_fit(array_t self)
{
	assert(self);
	if( self->capacity != self->count ){
		array_setCapacity(self, self->count);
	}
}

void* array_at(const array_t self, size_t index)
{
	assert(self);
	return index < self->count ? self->data + self->itemSize * index : NULL;
}

size_t array_add(array_t self, const void* data)
{
	assert(self && data);
	if( array_checkCapacity(self) != 0){
		return 0;
	}

	void* dest = self->data + self->itemSize * self->count;
	memcpy(dest, data, self->itemSize);
	return ++self->count;
}

size_t array_insert(array_t self, size_t index, const void* data)
{
	assert(self && data);
	if( index > self->count ){
		errno = EINVAL;
		return INVALID_ARRAY_INDEX;
	}

	if( array_checkCapacity(self) != 0){
		return INVALID_ARRAY_INDEX;
	}

	unsigned char* dest = self->data + (self->itemSize * index);

	memmove(dest + self->itemSize, dest,
			(self->count - index) * self->itemSize);

	memcpy(dest, data, self->itemSize);
	++self->count;
	return index;
}

void array_erase(array_t self, size_t index)
{
	assert(self);
	if( index < self->count ){
		--self->count;

		unsigned char* dest = self->data + self->itemSize * index;
		if( self->destructor ){
			self->destructor(dest);
		}
		memmove(dest, dest + self->itemSize,
				self->itemSize * (self->count - index));
	}
}

size_t array_find(const array_t self, const void* data)
{
	return array_findInIf(self, data, (FCompare)memcmp, 0, self->count);
}

size_t array_findIf(const array_t self, const void* data, FCompare fc)
{
	return array_findInIf(self, data, fc, 0, self->count);
}

size_t array_findIn(const array_t self, const void* data,
		size_t start, size_t  end)
{
	return array_findInIf(self, data, (FCompare)memcmp, start, end);
}

size_t array_findInIf(const array_t self, const void* data,
		FCompare f, size_t start, size_t end)
{
	assert(self && data);

	if( self->count < end ){
		end = self->count;
	}

	while( start < end ){
		if( f(self->data + (self->itemSize * start), data, self->itemSize) == cmpEQUAL){
			return start;
		}

		start++;
	}

	return INVALID_ARRAY_INDEX;
}

void array_remove(array_t self, const void* data)
{
	array_removeIf(self, data, (FCompare)memcmp);
}

void array_removeIf(array_t self, const void* data, FCompare f)
{
	assert(self && data);

	size_t index = array_findIf(self, data, f);
	while(index != INVALID_ARRAY_INDEX){
		array_erase(self, index);
		index = array_findInIf(self, data, f, index, self->count);
	}
}
