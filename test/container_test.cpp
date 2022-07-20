#include "container_test.h"

void create_TestStruct(void* ms)
{
	new(ms) TestStruct();
}

void delete_TestStruct(void* ms)
{
	reinterpret_cast<TestStruct*>(ms)->~TestStruct();
}

enum ECompare compare_TestStruct(const TestStruct* ts1, const TestStruct* ts2)
{
	return *ts1 == *ts2 ? cmpEQUAL : (*ts1 > *ts2 ? cmpGREATER : cmpLESS);
}
