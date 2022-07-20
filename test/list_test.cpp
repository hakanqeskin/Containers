//============================================================================
// Name        : list.cpp
// Author      : hakan keskin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <containers/list.h>
#include "container_test.h"

#include <iostream>
#include <cstring>

#include <list>
#include <algorithm>

static bool list101();
static bool list102();
static bool list103();
static bool list104();

int main(int argc, char* argv[])
{//./test_list [101]
	FTestCase test_f = NULL;
	if( argc == 2 ){
		if( !strcmp(argv[1], "101") ){
			test_f = list101;
		}
		else if( !strcmp(argv[1], "102") ){
			test_f = list102;
		}
		else if( !strcmp(argv[1], "103") ){
			test_f = list103;
		}
		else if( !strcmp(argv[1], "104") ){
			test_f = list104;
		}
	}

	if( !test_f ){
		if( !list101() ){
			return 1;
		}

		if( !list102() ){
			return 1;
		}

		if( !list103() ){
			return 1;
		}

		if( !list104() ){
			return 1;
		}
	}
	else{
		if( !test_f()){
			return 1;
		}
	}

	std::cout<<"list tests finished"<<std::endl;
	return 0;
}

static void print_List(list_t L)
{
	std::cout<<"*********LIST****************\n";
	TestStruct* pts;
	ListIterator* it = list_first(L);
	while( it ){
		pts = (TestStruct*)listIt_data(it);
		std::cout<<*pts<<"\n";
		it = listIt_next(it);
	}
	std::cout<<"********LIST-END*****************\n";
}

static void print_STLContainer(const std::list<TestStruct>& C)
{
	std::cout<<"*************************\n";
	for(const TestStruct& m : C){
		std::cout<<m<<"\n";
	}
	std::cout<<"*************************\n";
}

static void initContainers(std::list<TestStruct>& C, list_t L)
{
	TestStruct ts;
	for(unsigned I = 0; I < TEST_CONTAINER_SIZE; ++I){
		ts.i_value = I;
		ts.f_value = I * 3.0 / 2;

		C.push_back(ts);
		list_add(L, &ts);
	}

	//print_List(L);
	//print_STLContainer(C);
}

static bool compareContent(const std::list<TestStruct>& C, const list_t L)
{
	if( list_size(L) != C.size() ){
		std::cout<<"Size different. List:"
				<<list_size(L)
				<<" STL:"<<C.size()<<std::endl;

		return false;
	}

	typedef std::list<TestStruct>::const_iterator TTestStruct;
	TTestStruct cit = C.begin();

	TestStruct* pts;
	ListIterator* it = list_first(L);
	unsigned index = 0;
	while( it ){
		pts = (TestStruct*)listIt_data(it);
		if( *cit != *pts ){
			std::cout<<"FAILED INDEX:"<<index<<std::endl;

			print_STLContainer(C);
			print_List(L);
			return false;
		}

		it = listIt_next(it);
		++cit;
		++index;
	}

	print_List(L);
	return true;
}

static bool list101()
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::list<TestStruct> C;
	list_t L = list_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	list_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static void insertDataEvenIndex(std::list<TestStruct>& C, list_t L)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::list<TestStruct>::iterator TTestStruct;
	TTestStruct cit = C.begin();
	ListIterator* it = list_first(L);

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		cit = C.insert(cit, ts);
		++cit;
		++cit;
		it = list_insert(L, it, &ts);
		it = listIt_next(it);
		it = listIt_next(it);
	}
}

static void insertDataOddIndex(std::list<TestStruct>& C, list_t L)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::list<TestStruct>::iterator TTestStruct;
	TTestStruct cit = C.begin();
	ListIterator* it = list_first(L);

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		cit = C.insert(++cit, ts);
		++cit;

		it = listIt_next(it);
		it = list_insert(L, it, &ts);
		it = listIt_next(it);
	}
}

static bool list102()
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::list<TestStruct> C;
	list_t L = list_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);


	initContainers(C, L);
	insertDataEvenIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	list_clear(L);

	initContainers(C, L);
	insertDataOddIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	list_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static bool list103()
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::list<TestStruct> C;
	list_t L = list_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, L);
	insertDataEvenIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	typedef std::list<TestStruct>::iterator TTestStruct;
	TTestStruct cit = C.begin();
	ListIterator* it = list_first(L);

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		cit = C.erase(cit);
		++cit;
		it = list_erase(L, it);
		it = listIt_next(it);
	}

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	list_clear(L);

	initContainers(C, L);
	insertDataOddIndex(C, L);

	if( !compareContent(C, L)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	cit = C.begin();
	it = list_first(L);
	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		cit = C.erase(++cit);
		//++cit;
		it = listIt_next(it);
		it = list_erase(L, it);

	}

	print_STLContainer(C);

	print_List(L);

	list_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static bool findInBoth(const std::list<TestStruct>& C, const list_t L)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::list<TestStruct>::const_iterator TTestStruct;

	TTestStruct cit = std::find(C.begin(), C.end(), ts);
	ListIterator* it = list_find(L, &ts);
	while( it ){
		++cit;
		it = listIt_next(it);

		if( it ){
			if( *cit != *(reinterpret_cast<TestStruct*>(listIt_data(it))) ){
				std::cout<<"CIT:"<<*cit
						<<" LIT:"<<*(reinterpret_cast<TestStruct*>(listIt_data(it)))
						<<std::endl;

				return false;
			}
		}

		cit = std::find(cit, C.end(), ts);
		it = list_findIn(L, &ts, it, NULL);
	}

	return true;
}

static bool list104()
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::list<TestStruct> C;
	list_t L = list_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

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
	list_clear(L);

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

	list_delete(L);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}
