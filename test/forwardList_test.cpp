/*
 * test_forwardList.cpp
 *
 *  Created on: Jan 11, 2022
 *      Author: hakan.keskin
 */

#include <containers/forwardList.h>
#include "container_test.h"

#include <iostream>
#include <cstring>

#include <forward_list>
#include <iterator>
#include <algorithm>


static std::size_t m_size = 0;

static bool forwardList101(void);
static bool forwardList102(void);
static bool forwardList103(void);
static bool forwardList104(void);


int main(int argc, char* argv[])
{//./array_example 101
	FTestCase test_f = NULL;
	if( argc == 2 ){
		if( !strcmp(argv[1], "101") ){
			test_f = forwardList101;
		}
		else if( !strcmp(argv[1], "102") ){
			test_f = forwardList102;
		}
		else if( !strcmp(argv[1], "103") ){
			test_f = forwardList103;
		}
		else if( !strcmp(argv[1], "104") ){
			test_f = forwardList104;
		}
	}

	if( !test_f ){
		if( !forwardList101()){
			return 1;
		}

		if( !forwardList102()){
			return 1;
		}

		if( !forwardList103()){
			return 1;
		}

		if( !forwardList104()){
			return 1;
		}
}
	else{
		if( !test_f()){
			return 1;
		}
	}

	std::cout<<"forward list tests finished"<<std::endl;
	return 0;
}

static void print_FList(const flist_t L)
{
	std::cout<<"*********FLIST****************\n";
	TestStruct* pts;
	struct Iterator* it = flist_first(L);
	while( it ){
		pts = (TestStruct*)flistIt_data(it);
		std::cout<<pts->str<<"\n";
		it = flistIt_next(it);
	}
	std::cout<<"*********FLIST-END****************\n";
}

static void print_STLContainer(const std::forward_list<TestStruct>& C)
{
	std::cout<<"********STL*****************\n";
	for(const TestStruct& ts : C){
		std::cout<<ts<<"\n";
	}
	std::cout<<"********STL-END*****************\n";
}

static void initContainers(std::forward_list<TestStruct>& C, flist_t L)
{
	TestStruct ts;
	for(unsigned I = 0; I < TEST_CONTAINER_SIZE; ++I){
		ts_init(&ts, I , I * 3.0 / 2);

		C.push_front(ts);
		flist_add(L, &ts);
	}

	m_size = TEST_CONTAINER_SIZE;
}

static bool compareContent(const std::forward_list<TestStruct>& C,
		const flist_t L)
{
	if( flist_size(L) != m_size ){
		std::cout<<"Size different. Array:"
				<<flist_size(L)
				<<" STL:"<<m_size<<std::endl;
		return false;
	}

	typedef std::forward_list<TestStruct>::const_iterator TTestStruct;
	TTestStruct cit = C.begin();

	std::size_t index = 0;
	TestStruct* pts;
	struct Iterator* fit = flist_first(L);
	while( fit ){
		pts = (TestStruct*)flistIt_data(fit);
		if( *cit != *pts ){
			std::cout<<"FAILED INDEX:"<<index<<std::endl;
			print_STLContainer(C);
			print_FList(L);
			return false;
		}

		fit = flistIt_next(fit);
		++cit;
		++index;
	}

	print_FList(L);
	return true;
}

static bool forwardList101(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::forward_list<TestStruct> C;
	flist_t L = flist_create(sizeof(TestStruct), (FDestructor)ts_release);

	initContainers(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	flist_delete(L);

	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static void insertDataEvenIndex(std::forward_list<TestStruct>& C, flist_t L)
{
	TestStruct ts;
	ts_init(&ts, TEST_CONTAINER_SIZE, TEST_CONTAINER_SIZE * 3.0 / 2);

	C.push_front(ts);
	++m_size;

	typedef std::forward_list<TestStruct>::iterator TTestStruct;
	TTestStruct cit = C.begin();
	struct Iterator* it = flist_add(L, &ts);

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		ts_init(&ts, TEST_CONTAINER_SIZE, TEST_CONTAINER_SIZE * 3.0 / 2);
		cit = C.insert_after(++cit, ts);
		it = flistIt_next(it);
		it = flist_insert(L, it, &ts);

		++m_size;
	}
}

static void insertDataOddIndex(std::forward_list<TestStruct>& C, flist_t L)
{
	typedef std::forward_list<TestStruct>::iterator TTestStruct;

	TTestStruct cit = C.begin();
	struct Iterator* it = flist_first(L);

	TestStruct ts;
	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		ts_init(&ts, TEST_CONTAINER_SIZE, TEST_CONTAINER_SIZE * 3.0 / 2);
		cit = C.insert_after(cit, ts);
		it = flist_insert(L, it, &ts);
		++cit;
		it = flistIt_next(it);
		++m_size;
	}
}

static bool forwardList102(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::forward_list<TestStruct> C;
	flist_t L = flist_create(sizeof(TestStruct), (FDestructor)ts_release);

	initContainers(C, L);
	insertDataEvenIndex(C, L);


	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	flist_clear(L);

	initContainers(C, L);
	insertDataOddIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	flist_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
    return true;
}

static bool forwardList103(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::forward_list<TestStruct> C;
	flist_t L = flist_create(sizeof(TestStruct), (FDestructor)ts_release);

	initContainers(C, L);
	insertDataEvenIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.pop_front();
	--m_size;

	typedef std::forward_list<TestStruct>::iterator TTestStruct;
	TTestStruct cit = C.begin();
	struct Iterator* it = flist_eraseFirst(L);
	while(it){
		cit = C.erase_after(cit);
		it = flist_eraseAfter(L, it);
		--m_size;
	}

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	flist_clear(L);

	initContainers(C, L);
	insertDataOddIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	cit = C.begin();
	it = flist_first(L);
	while(it){
		cit = C.erase_after(cit);
		it = flist_eraseAfter(L, it);
		--m_size;
	}

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	flist_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
    return true;
}

static bool findInBoth(const std::forward_list<TestStruct>& C, const flist_t L)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::forward_list<TestStruct>::const_iterator TTestStruct;

	TTestStruct cit = std::find(C.begin(), C.end(), ts);
	struct Iterator* it = flist_find(L, &ts);
	while( it ){
		++cit;
		it = flistIt_next(it);

		if( it ){
			if( *cit != *(reinterpret_cast<TestStruct*>(flistIt_data(it))) ){
				std::cout<<"CIT:"<<*cit
						<<" FIT:"<<*(reinterpret_cast<TestStruct*>(flistIt_data(it)))
						<<std::endl;

				return false;
			}
		}

		cit = std::find(cit, C.end(), ts);
		it = flist_findIn(L, &ts, it, NULL);
	}

	return true;
}

static bool forwardList104(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::forward_list<TestStruct> C;
	flist_t L = flist_create(sizeof(TestStruct), (FDestructor)ts_release);

	initContainers(C, L);
	insertDataEvenIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	if( !findInBoth(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	flist_clear(L);

	initContainers(C, L);
	insertDataOddIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	if( !findInBoth(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	flist_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
    return true;
}
