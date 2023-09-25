/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 25 Sep, 2023
**
** @copyright Copyright (C) 2023 James Till Matta
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_THREADS_READERTHREAD_H
#define DGSSORT_SRC_THREADS_READERTHREAD_H

// includes for C system headers
// includes for C++ system headers
#include<string>
#include<cstdint>
// includes from other libraries
#include<zlib.h>
// includes from DgsSort
#include"BufferQueue.h"
#include"Utility/Misc.h"

namespace Threading
{

static const uint64_t GzipInternalBufferSize = 64*1048576;
static const uint64_t ReadSizeInBytes = 32 * 1048576;
static const uint64_t BufferOversize = 16384; // must be larger than 4k and a multiple of 64
static const uint64_t TotalBufferSize = ReadSizeInBytes + BufferOversize;
static const uint64_t BlocksInFlight = 16; // must be larger than 4k and a multiple of 64

struct BufferBlock
{
    BufferBlock()
    {
        buffer = Utility::cacheAlignedAllocGoodSize(TotalBufferSize);
    }
    ~BufferBlock()
    {
        std::free(buffer);
    }
    uint64_t used{0};
    uint64_t startInd{BufferOversize};
    uint8_t* buffer{nullptr};
    void reset()
    {
        used = 0;
        startInd = BufferOversize;
    }
};

/*!
* @brief
*/
class ReaderThread
{
    using QueueType = BufferQueue<BufferBlock, BlocksInFlight>;
public:
    ReaderThread(std::string const& inFileName, QueueType& queue);
    ~ReaderThread();

    void run();

private:
    QueueType& outQueue;
    gzFile inputFile;
};

} // namespace Threading

#endif  // DGSSORT_SRC_THREADS_READERTHREAD_H
