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
#ifndef DGSSORT_SRC_UTILITY_MISC_H
#define DGSSORT_SRC_UTILITY_MISC_H

// includes for C system headers
// includes for C++ system headers
#include<cstdint>
// includes from other libraries
// includes from DgsSort

namespace Utility
{

static const uint64_t CacheLineAlignment = 64;
static const uint64_t CacheLineAlignmentMask = 63;

uint8_t* cacheAlignedAlloc(uint64_t & size);

} // namespace Utility

#endif  // DGSSORT_SRC_UTILITY_MISC_H
