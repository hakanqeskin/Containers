/*
 * forwardList.h
 *
 *  Created on: Jan 11, 2022
 *      Author: hakan.keskin
 */

#ifndef CONTAINERS_FORWARDLIST_H_
#define CONTAINERS_FORWARDLIST_H_

#include "container.h"

/*
 Single linked list (Forward list)
 _________     _________           _________     _________
|    |    |   |    |    |         |    |    |   |    |    |
|Data|Next|-->|Data|Next|-->......|Data|Next|-->|Data|Next|-->NULL
|____|____|   |____|____|         |____|____|   |____|____|
 ____                                              |
|    |                                             |
|Last|----------------------------------------------
|____|
*/

#ifdef __cplusplus
extern "C" {
#endif

struct Iterator;

void* flistIt_data(struct Iterator*);
struct Iterator* flistIt_next(struct Iterator*);

typedef struct ForwardList* flist_t;

flist_t flist_create(size_t itemSize, FDestructor);
void flist_delete(flist_t);

void flist_clear(flist_t);

size_t flist_size(const flist_t);

struct Iterator* flist_first(const flist_t);

/*inserts an element to the beginning*/
struct Iterator* flist_add(flist_t, const void*);

/*insert data after iterator*/
struct Iterator* flist_insert(flist_t, struct Iterator*, const void*);

/*erases iterator's next, return next iterator*/
struct Iterator* flist_eraseAfter(flist_t, struct Iterator*);

struct Iterator* flist_eraseFirst(flist_t);

struct Iterator* flist_find(const flist_t, const void*);
struct Iterator* flist_findIf(const flist_t, const void*, FCompare);
struct Iterator* flist_findIn(const flist_t, const void*,
		struct Iterator* start, struct Iterator* end);

/*find in range [start,end)*/
struct Iterator* flist_findInIf(const flist_t, const void*, FCompare,
		struct Iterator* start, struct Iterator* end);


/*removes elements satisfying specific criteria*/
void flist_remove(flist_t, const void*);
void flist_removeIf(flist_t, const void*, FCompare);

#ifdef __cplusplus
}
#endif

#endif /* CONTAINERS_FORWARDLIST_H_ */
