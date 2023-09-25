/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 24 Sep, 2023
**
** @copyright Copyright (C) 2023 James Till Matta
**
********************************************************************************
*******************************************************************************/
#include"Misc.h"
// includes for C system headers
// includes for C++ system headers
#include<cstdlib>
// includes from other libraries
// includes from DgsSort

namespace Utility
{

uint8_t* cacheAlignedAlloc(uint64_t& size)
{
    // ensure the size is a multiple of 64 or make it so
    // for powers of 2 bit mask is the same as modulus and much faster
    if((size & CacheLineAlignmentMask) != 0)
    {
        size += CacheLineAlignment - (size & CacheLineAlignmentMask);
    }
    return static_cast<uint8_t*>(std::aligned_alloc(CacheLineAlignment, size));
}

} // namespace Utility

