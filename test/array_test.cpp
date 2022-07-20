#include <containers/array.h>
#include "container_test.h"

#include <iostream>
#include <cstring>

#include <vector>
#include <algorithm>


static bool array101(void);
static bool array102(void);
static bool array103(void);
static bool array104(void);


int main(int argc, char* argv[])
{//./array_example 101
	FTestCase test_f = NULL;
	if( argc == 2 ){
		if( !strcmp(argv[1], "101") ){
			test_f = array101;
		}
		else if( !strcmp(argv[1], "102") ){
			test_f = array102;
		}
		else if( !strcmp(argv[1], "103") ){
			test_f = array103;
		}
		else if( !strcmp(argv[1], "104") ){
			test_f = array104;
		}
	}

	if( !test_f ){
		if( !array101()){
			return 1;
		}

		if( !array102()){
			return 1;
		}

		if( !array103()){
			return 1;
		}

		if( !array104()){
			return 1;
		}
	}
	else{
		if( !test_f()){
			return 1;
		}
	}

	std::cout<<"array tests finished"<<std::endl;
	return 0;
}

static void print_Array(const array_t A)
{
	std::cout<<"*********ARRAY****************\n";
	TestStruct* pts;
	size_t count = array_size(A);
	for(size_t I = 0; I < count; ++I){
		pts = (TestStruct*)array_at(A, I);
		std::cout<<*pts<<"\n";
	}
	std::cout<<"********ARRAY-END*****************\n";
}

static void print_STLContainer(const std::vector<TestStruct>& C)
{
	std::cout<<"********STL*****************\n";
	for(const TestStruct& ts : C){
		std::cout<<ts<<"\n";
	}
	std::cout<<"********STL-END*****************\n";
}

static void initContainers(std::vector<TestStruct>& C, array_t A)
{
	TestStruct ts;
	for(unsigned I = 0; I < TEST_CONTAINER_SIZE; ++I){
		ts.i_value = I;
		ts.f_value = I * 3.0 / 2;

		C.push_back(ts);
		array_add(A, &ts);
	}

	//print_STLContainer(V);
	//print_Array(A);
}

static bool compareContent(const std::vector<TestStruct>& C, const array_t A)
{
	if( array_size(A) != C.size() ){
		std::cout<<"Size different. Array:"
				<<array_size(A)
				<<" STL:"<<C.size()<<std::endl;
		return false;
	}

	typedef std::vector<TestStruct>::const_iterator TTestStruct;
	TTestStruct cit = C.begin();

	TestStruct* pts;
	size_t count = array_size(A);
	for(size_t I = 0; I < count; ++I){
		pts = (TestStruct*)array_at(A, I);
		if( *cit != *pts ){
			std::cout<<"FAILED INDEX:"<<I<<std::endl;

			print_STLContainer(C);
			print_Array(A);
			return false;
		}
		++cit;
	}

	print_Array(A);
	return true;
}

static bool array101(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::vector<TestStruct> C;
	array_t A = array_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	array_delete(A);

	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static void insertDataEvenIndex(std::vector<TestStruct>& C, array_t A)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::vector<TestStruct>::iterator TTestStruct;
	TTestStruct it = C.begin();
	size_t index = 0;

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		it = C.insert(it, ts);
		it += 2;
		index = array_insert(A, index, &ts);
		index += 2;
	}
}

static void insertDataOddIndex(std::vector<TestStruct>& C, array_t A)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::vector<TestStruct>::iterator TTestStruct;
	TTestStruct it = C.begin();
	size_t index = 0;

	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		it = C.insert(++it, ts);
		++it;
		index = array_insert(A, ++index, &ts);
		++index;
	}
}

static bool array102(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::vector<TestStruct> C;
	array_t A = array_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, A);
	insertDataEvenIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	array_clear(A);

	initContainers(C, A);
	insertDataOddIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	array_delete(A);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static bool array103(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::vector<TestStruct> C;
	array_t A = array_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, A);
	insertDataEvenIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	typedef std::vector<TestStruct>::iterator TTestStruct;
	TTestStruct it = C.begin();
	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		it = C.erase(it);
		++it;
		array_erase(A, I);
	}

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	C.clear();
	array_clear(A);

	initContainers(C, A);
	insertDataOddIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	it = C.begin();
	for(std::size_t I = 0; I < TEST_CONTAINER_SIZE; ++I){
		it = C.erase(++it);
		array_erase(A, I + 1);
	}

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	array_delete(A);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}

static bool findInBoth(const std::vector<TestStruct>& C, const array_t A)
{
	TestStruct ts;
	ts.i_value = TEST_CONTAINER_SIZE;
	ts.f_value = ts.i_value * 3.0 / 2;

	typedef std::vector<TestStruct>::const_iterator TTestStruct;

	TTestStruct it = std::find(C.begin(), C.end(), ts);
	size_t index = array_find(A, &ts);
	while( index != INVALID_ARRAY_INDEX ){
		size_t pos = it - C.begin();
		if( pos != index ){
			std::cout<<"POS:"<<pos<<" INDEX:"<<index<<std::endl;
			return false;
		}

		it = std::find(++it, C.end(), ts);
		index = array_findIn(A, &ts, ++index, TEST_CONTAINER_SIZE * 2);
	}

	return true;
}

static bool array104(void)
{
	std::cout<<"BEGIN "<<__func__<<std::endl;
	std::vector<TestStruct> C;
	array_t A = array_create(sizeof(TestStruct),
			create_TestStruct, delete_TestStruct);

	initContainers(C, A);
	insertDataEvenIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	if( !findInBoth(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}


	C.clear();
	array_clear(A);

	initContainers(C, A);
	insertDataOddIndex(C, A);

	if( !compareContent(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	if( !findInBoth(C, A)){
		std::cout<<"TEST FILED "<<__func__<<":"<<__LINE__<<std::endl;
		return false;
	}

	array_delete(A);
	std::cout<<"END "<<__func__<<std::endl;
	return true;
}
