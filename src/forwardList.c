/*
 * forwardList.c
 *
 *  Created on: Jan 11, 2022
 *      Author: hakan.keskin
 */

#include "include/containers/forwardList.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

struct Iterator{
	struct Iterator* next;
	unsigned char data[0];
};

struct ForwardList{
	size_t itemSize;
	size_t count;
	FDestructor destructor;

	struct Iterator first;
};

static struct Iterator* flist_createIt(flist_t self, const void* data)
{
	struct Iterator* it = malloc(sizeof(struct Iterator) + self->itemSize);
	if( it ){
		memcpy(it->data, data, self->itemSize);
	}

	it->next = NULL;
	return it;
}

static void flist_deleteIt(flist_t self, struct Iterator* it)
{
	if( self->destructor ){
		self->destructor(it->data);
	}

	free(it);
}

void* flistIt_data(struct Iterator* it)
{
	assert(it);
	return it->data;
}

struct Iterator* flistIt_next(struct Iterator* it)
{
	assert(it);
	return it->next;
}

static void flist_reset(flist_t self)
{
	self->first.next = NULL;
	self->count = 0;
}

static void flist_deleteItems(flist_t self)
{
	struct Iterator* prev;
	struct Iterator* it = self->first.next;
	while( it ){
		prev = it;
		it = it->next;
		flist_deleteIt(self, prev);
	}
}

flist_t flist_create(size_t itemSize, FDestructor destructor)
{
	if( !itemSize ){
		errno = EINVAL;
		return NULL;
	}

	flist_t self = malloc(sizeof(struct ForwardList));
	if(self){
		flist_reset(self);
		self->destructor = destructor;
		self->itemSize = itemSize;
	}

	return self;
}

void flist_delete(flist_t self)
{
	if( self && self->count ){
		flist_deleteItems(self);
	}

	free(self);
}

void flist_clear(flist_t self)
{
	assert(self);
	if( self->count ){
		flist_deleteItems(self);
		flist_reset(self);
	}
}

size_t flist_size(const flist_t self)
{
	assert(self);
	return self->count;
}

struct Iterator* flist_first(const flist_t self)
{
	assert(self);
	return self->first.next;
}

struct Iterator* flist_add(flist_t self, const void* data)
{
	assert(self && data);
	struct Iterator* it = flist_createIt(self, data);
	if( it ){
		it->next = self->first.next;
		self->first.next = it;
		++self->count;
	}
	return it;
}

struct Iterator* flist_insert(flist_t self,
		struct Iterator* prev, const void* data)
{
	assert(self && prev && data);
	struct Iterator* it = flist_createIt(self, data);
	if( it ){
		it->next = prev->next;
		prev->next = it;
		++self->count;
	}
	return it;
}

struct Iterator* flist_eraseAfter(flist_t self, struct Iterator* it)
{
	assert(self && it);

	struct Iterator* tmp = it->next;
	it->next = tmp->next;
	--self->count;
	flist_deleteIt(self, tmp);
	return it->next;
}

struct Iterator* flist_eraseFirst(flist_t self)
{
	assert(self);

	struct Iterator* tmp = self->first.next;
	self->first.next = tmp->next;
	--self->count;
	flist_deleteIt(self, tmp);
	return self->first.next;
}

struct Iterator* flist_find(const flist_t self, const void* data)
{
	return flist_findInIf(self, data, (FCompare)memcmp, self->first.next, NULL);
}

struct Iterator* flist_findIf(const flist_t self, const void* data, FCompare f)
{
	return flist_findInIf(self, data, f, self->first.next, NULL);
}

struct Iterator* flist_findIn(const flist_t self, const void* data,
		struct Iterator* start, struct Iterator* end)
{
	return flist_findInIf(self, data, (FCompare)memcmp, start, end);
}

struct Iterator* flist_findInIf(const flist_t self, const void* data, FCompare f,
		struct Iterator* start, struct Iterator* end)
{
	assert(self && data);
	while( start && start != end ){
		if( f(start->data, data, self->itemSize) == cmpEQUAL ){
			break;
		}

		start = start->next;
	}

	return start;
}


void flist_remove(flist_t self, const void* data)
{
	flist_removeIf(self, data, (FCompare)memcmp);
}

void flist_removeIf(flist_t self, const void* data, FCompare f)
{
	assert(self && data);

	if( self->count ){
		struct Iterator* prev = self->first.next;
		struct Iterator* it = prev->next;

		while( it ){
			if( f(it->data, data, self->itemSize) == cmpEQUAL){
				it = flist_eraseAfter(self, prev);
			}
			else{
				prev = it;
				it = it->next;
			}
		}

		if( f(self->first.next->data, data, self->itemSize) == cmpEQUAL){
			flist_eraseFirst(self);
		}
	}
}
