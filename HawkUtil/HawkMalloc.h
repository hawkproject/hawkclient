#ifndef HAWK_MALLOC_H
#define HAWK_MALLOC_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/* Malloc相关操作封装													*/
	/************************************************************************/
	//内存分配
	void*  UTIL_API HawkMalloc(Size_t iSize);

	//内存分配
	void*  UTIL_API HawkCalloc(Size_t iNum, Size_t iSize);

	//内存重分配
	void*  UTIL_API HawkRealloc(void* pMem, Size_t iSize);

	//内存释放
	void   UTIL_API HawkFree(void* pMem);
}
#endif
