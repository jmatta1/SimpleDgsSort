/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 ??
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_UTILITY_ENDIANHANDLING_H
#define DGSSORT_SRC_UTILITY_ENDIANHANDLING_H

// includes for C system headers
#include <machine/endian.h>
#ifdef __APPLE__
    #include <libkern/OSByteOrder.h>
#endif
// includes for C++ system headers
// includes from other libraries
// includes from DgsSort

#ifdef __APPLE__
    #define htobe16(x) OSSwapHostToBigInt16(x)
    #define htole16(x) OSSwapHostToLittleInt16(x)
    #define be16toh(x) OSSwapBigToHostInt16(x)
    #define le16toh(x) OSSwapLittleToHostInt16(x)

    #define htobe32(x) OSSwapHostToBigInt32(x)
    #define htole32(x) OSSwapHostToLittleInt32(x)
    #define be32toh(x) OSSwapBigToHostInt32(x)
    #define le32toh(x) OSSwapLittleToHostInt32(x)

    #define htobe64(x) OSSwapHostToBigInt64(x)
    #define htole64(x) OSSwapHostToLittleInt64(x)
    #define be64toh(x) OSSwapBigToHostInt64(x)
    #define le64toh(x) OSSwapLittleToHostInt64(x)

#endif

#endif  // DGSSORT_SRC_UTILITY_ENDIANHANDLING_H