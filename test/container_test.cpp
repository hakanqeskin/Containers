#include "container_test.h"

#ifdef _MSC_VER
#include <cstdarg>

int asprintf(char** strp, const char* fmt, ...)
{
	if (!strp) {
		return -1;
	}

	// Determine required size
	va_list ap;
	va_start(ap, fmt);
	int size = vsnprintf(NULL, 0, fmt, ap);
	va_end(ap);

	if (size < 0) {
		return -1;
	}

	//for null terminated
	*strp = (char*)malloc(++size);
	if (!*strp) {
		return -1;
	}

	va_start(ap, fmt);
	size = vsnprintf(*strp, size, fmt, ap);
	va_end(ap);

	if (size < 0) {
		free(*strp);
	}

	return size;
}
#endif

void ts_init(struct TestStruct* ts, int iv, float fv)
{
	if( ts ){
		ts->i_value = iv;
		ts->f_value = fv;
		asprintf(&ts->str, "INT:%d FLOAT:%f", iv, fv);
	}
}

void ts_release(struct TestStruct* ts)
{
	if( ts ){
		free(ts->str);
	}
}
/*
void create_TestStruct(void* ms)
{
	new(ms) TestStruct();
}

void delete_TestStruct(void* ms)
{
	reinterpret_cast<TestStruct*>(ms)->~TestStruct();
}
*/
enum ECompare compare_TestStruct(const TestStruct* ts1, const TestStruct* ts2)
{
	return *ts1 == *ts2 ? cmpEQUAL : (*ts1 > *ts2 ? cmpGREATER : cmpLESS);
}

/*
 int asprintf(char** strp, const char* fmt, ...)
	{
		if (!strp) {
			return -1;
		}

		// Determine required size
		va_list ap;
		va_start(ap, fmt);
		int size = vsnprintf(NULL, 0, fmt, ap);
		va_end(ap);

		if (size < 0) {
			return -1;
		}

		//for null terminated
		*strp = (char*)malloc(++size);
		if (!*strp) {
			return -1;
		}

		va_start(ap, fmt);
		size = vsnprintf(*strp, size, fmt, ap);
		va_end(ap);

		if (size < 0) {
			free(*strp);
		}

		return size;
	}
 */
