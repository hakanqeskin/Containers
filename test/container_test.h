/*
 * container_test.h
 *
 *  Created on: Feb 8, 2022
 *      Author: hakan.keskin
 */

#ifndef CONTAINERS_TEST_H_
#define CONTAINERS_TEST_H_

#include <containers/container.h>

#include <ostream>

#define TEST_CONTAINER_SIZE		17

typedef bool (*FTestCase)(void);

struct TestStruct
{
	int i_value;
	float f_value;

    bool operator==(const TestStruct& t)const
    {
    	return this->i_value == t.i_value && this->f_value == t.f_value;
    }

    bool operator!=(const TestStruct& t)const
    {
    	return this->i_value != t.i_value || this->f_value != t.f_value;
    }

    bool operator >(const TestStruct& t)const
    {
    	return this->i_value == t.i_value ?
    			this->f_value > t.f_value : this->i_value > t.i_value;
    }

    bool operator <(const TestStruct& t)const
    {
    	return this->i_value == t.i_value ?
    			this->f_value < t.f_value : this->i_value < t.i_value;
    }

    friend std::ostream& operator<<(std::ostream& os, const TestStruct& ms)
	{
		os<<"INT:"<<ms.i_value<<" FLOAT:"<<ms.f_value;
		return os;
	}
};

void create_TestStruct(void*);
void delete_TestStruct(void*);

enum ECompare compare_TestStruct(const TestStruct*, const TestStruct*);

#endif /* CONTAINERS_TEST_H_ */
