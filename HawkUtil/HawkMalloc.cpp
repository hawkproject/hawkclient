#include "HawkMalloc.h"

namespace Hawk
{
	void* HawkMalloc(Size_t iSize)
	{
		return malloc(iSize);
	}

	void* HawkCalloc(Size_t iNum, Size_t iSize)
	{
		return calloc(iNum, iSize);
	}

	void* HawkRealloc(void* pMem, Size_t iSize)
	{
		return realloc(pMem, iSize);
	}

	void  HawkFree(void* pMem)
	{
		free(pMem);
	}
}
