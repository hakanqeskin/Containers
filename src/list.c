/*
 * list.c
 *
 *  Created on: Jan 7, 2022
 *      Author: hakan.keskin
 */


#include "include/containers/list.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>


struct ListIterator{
	struct ListIterator* next;
	struct ListIterator* prev;
	unsigned char data[0];
};

struct List{
	size_t itemSize;
	size_t count;
	FDestructor destructor;

	struct ListIterator first;
	struct ListIterator* last;
};

static struct ListIterator* list_createIt(list_t self, const void* data)
{
	struct ListIterator* it = malloc(sizeof(struct ListIterator) + self->itemSize);
	if( it ){
		memcpy(it->data, data, self->itemSize);
	}

	it->next = NULL;
	it->prev = NULL;
	return it;
}

static void list_deleteIt(list_t self, struct ListIterator* it)
{
	if( self->destructor ){
		self->destructor(it->data);
	}

	free(it);
}

void* listIt_data(struct ListIterator* it)
{
	assert(it);
	return it->data;
}

struct ListIterator* listIt_next(struct ListIterator* it)
{
	assert(it);
	return it->next;
}

static void list_reset(list_t self)
{
	self->first.next = NULL;
	self->first.prev = NULL;
	self->last = &self->first;
	self->count = 0;
}

static void list_deleteItems(list_t self)
{
	struct ListIterator* tmp;
	struct ListIterator* it = self->first.next;
	do{
		tmp = it;
		it = it->next;
		list_deleteIt(self, tmp);
	}while( it );
}

list_t list_create(size_t itemSize, FDestructor destructor)
{
	if( !itemSize ){
		errno = EINVAL;
		return NULL;
	}

	list_t self = malloc(sizeof(struct List));
	if(self){
		list_reset(self);
		self->itemSize = itemSize;
		self->destructor = destructor;
	}

	return self;
}

void list_delete(list_t self)
{
	if( self && self->count ){
		list_deleteItems(self);
	}

	free(self);
}

void list_clear(list_t self)
{
	if( self && self->count ){
		list_deleteItems(self);
		list_reset(self);
	}
}

size_t list_size(const list_t self)
{
	return self ? self->count : 0;
}

struct ListIterator* list_first(const list_t self)
{
	return self ? self->first.next : NULL;
}

struct ListIterator* list_add(list_t self, const void* data)
{
	assert(self && data);
	struct ListIterator* it = list_createIt(self, data);
	if( it ){
		it->prev = self->last;

		self->last->next = it;
		self->last = it;
		++self->count;
	}

	return it;
}

struct ListIterator* list_insert(list_t self, struct ListIterator* it,
		const void* data)
{
	assert(self && data);
	if( it ){
		struct ListIterator* prev = list_createIt(self, data);
		if( prev ){
			prev->prev = it->prev;
			it->prev->next = prev;

			prev->next = it;
			it->prev = prev;

			++self->count;
		}

		return prev;
	}

	return list_add(self, data);
}

struct ListIterator* list_erase(list_t self, struct ListIterator* it)
{
	assert(self && it);
	struct ListIterator* next = it->next;

	it->prev->next = next;
	if( next ){
		next->prev = it->prev;
	}
	else{
		self->last = it->prev;
	}

	/*it->prev->next = it->next;
	it->next->prev = it->prev;*/

	list_deleteIt(self, it);
	--self->count;

	return next;
}

struct ListIterator* list_find(const list_t self, const void* data)
{
	return list_findInIf(self, data, (FCompare)memcmp, self->first.next, NULL);
}

struct ListIterator* list_findIf(const list_t self, const void* data, FCompare f)
{
	return list_findInIf(self, data, f, self->first.next, NULL);
}

struct ListIterator* list_findIn(const list_t self, const void* data,
		struct ListIterator* start, struct ListIterator* end)
{
	return list_findInIf(self, data, (FCompare)memcmp, start, end);
}

struct ListIterator* list_findInIf(const list_t self, const void* data,
		FCompare f,	struct ListIterator* start, struct ListIterator* end)
{
	assert(self && data);

	while( start && start != end ){
		if( f(start->data, data, self->itemSize) == cmpEQUAL){
			break;
		}

		start = start->next;
	}

	return start;
}

void list_remove(list_t self, const void* data)
{
	list_removeIf(self, data, (FCompare)memcmp);
}

void list_removeIf(list_t self, const void* data, FCompare f)
{
	assert(self && data);
	struct ListIterator* it = list_findIf(self, data, f);
	while( it ){
		it = list_erase(self, it);
		it = list_findInIf(self, data, f, it, NULL);
	}
}
