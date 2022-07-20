/*
 * array.h
 *
 *  Created on: Jan 9, 2019
 *      Author: hakan.keskin
 */

#ifndef CONTAINERS_ARRAY_H_
#define CONTAINERS_ARRAY_H_

#include "container.h"

#define INVALID_ARRAY_INDEX			(size_t)-1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Array* array_t;

array_t array_create(size_t itemSize, FConstructor, FDestructor);
void array_delete(array_t);

void array_clear(array_t);

size_t array_size(const array_t);
void array_resize(array_t, size_t);

size_t array_capacity(const array_t);
void array_reserve(array_t, size_t);
void array_fit(array_t);

void* array_at(const array_t, size_t index);

/*add element to end.*/
size_t array_add(array_t, const void*);

/*insert element given posision.*/
size_t array_insert(array_t, size_t index, const void* data);

/*erase data at given posision.*/
void array_erase(array_t, size_t index);

size_t array_find(const array_t, const void*);
size_t array_findIf(const array_t, const void*, FCompare);

/*find in range [start,end)*/
size_t array_findIn(const array_t, const void*, size_t start, size_t end);
size_t array_findInIf(const array_t, const void*, FCompare,
		size_t start, size_t end);

/*removes elements satisfying specific criteria*/
void array_remove(array_t, const void*);
void array_removeIf(array_t, const void*, FCompare);

#ifdef __cplusplus
}
#endif

#endif /* CONTAINERS_ARRAY_H_ */
