/*
 * list.h
 *
 *  Created on: Jan 7, 2022
 *      Author: hakan.keskin
 */

#ifndef CONTAINERS_LIST_H_
#define CONTAINERS_LIST_H_

#include "container.h"

/*
 Double linked list
 _________     ______________           _________ ____     _____________
|    |    |-->|    |    |    |-->   -->|	|    |    |-->|    |    |    |-->NULL
|Data|Next|   |Prev|Data|Next|.........|Prev|Data|Next|   |Prev|Data|Next|
|____|____|<--|____|____|____|<--   <--|____|____|____|<--|____|____|____|
 ____                                              				  |
|    |                                             				  |
|Last|------------------------------------------------------------
|____|
*/

#ifdef __cplusplus
extern "C" {
#endif

struct ListIterator;

void* listIt_data(struct ListIterator*);
struct ListIterator* listIt_next(struct ListIterator*);

typedef struct List* list_t;

list_t list_create(size_t itemSize, FDestructor);
void list_delete(list_t);

void list_clear(list_t);

size_t list_size(const list_t);

struct ListIterator* list_first(const list_t);

/*add data to end of list*/
struct ListIterator* list_add(list_t, const void*);

/*insert data before iterator*/
struct ListIterator* list_insert(list_t, struct ListIterator*, const void*);

struct ListIterator* list_erase(list_t, struct ListIterator*);


struct ListIterator* list_find(const list_t, const void*);
struct ListIterator* list_findIf(const list_t, const void*, FCompare);
struct ListIterator* list_findIn(const list_t, const void*,
		struct ListIterator* start, struct ListIterator* end);

/*find in range [start,end)*/
struct ListIterator* list_findInIf(const list_t, const void*, FCompare,
		struct ListIterator* start, struct ListIterator* end);

/*removes elements satisfying specific criteria*/
void list_remove(list_t, const void*);
void list_removeIf(list_t, const void*, FCompare);

#ifdef __cplusplus
}
#endif

#endif /* CONTAINERS_LIST_H_ */

