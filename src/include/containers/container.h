/*
 * container.h
 *
 *  Created on: Feb 8, 2022
 *      Author: hakan.keskin
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*typedef void (*FConstructor)(void*);*/
typedef void (*FDestructor)(void*);

enum ECompare{
	cmpLESS = -1,
	cmpEQUAL,
	cmpGREATER
};

typedef enum ECompare (*FCompare)(const void*, const void*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* CONTAINER_H_ */
