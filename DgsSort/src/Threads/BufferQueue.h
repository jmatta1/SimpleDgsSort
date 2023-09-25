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
#ifndef DGSSORT_SRC_THREADS_BUFFERQUEUE_H
#define DGSSORT_SRC_THREADS_BUFFERQUEUE_H

// includes for C system headers
// includes for C++ system headers
#include<cstdint>
// includes from other libraries
#include<concurrentqueue/blockingconcurrentqueue.h>
// includes from DgsSort

namespace Threading
{

/*!
* @brief
*/
template <typename BufferType, uint64_t BuffCount>
class BufferQueue
{
    using QueueType = moodycamel::BlockingConcurrentQueue<BufferType*>;
public:
    BufferQueue()
        : buffArr(new BufferType[BuffCount])
    {
        for(uint64_t i=0; i<BuffCount; ++i)
        {
            emptyQueue.enqueue(buffArr + i);
        }
    }

    ~BufferQueue()
    {
        delete[] buffArr;
    }

    BufferType* getEmpty()
    {
        BufferType* temp;
        emptyQueue.wait_dequeue(temp);
        return temp;
    }

    void pushEmpty(BufferType* empty)
    {
        emptyQueue.enqueue(empty);
    }

    BufferType* getFull()
    {
        BufferType* temp;
        fullQueue.wait_dequeue(temp);
        return temp;
    }

    void pushFull(BufferType* full)
    {
        fullQueue.enqueue(full);
    }

private:
    QueueType fullQueue{};
    QueueType emptyQueue{};
    BufferType* buffArr{nullptr};
};

} // namespace Threading

#endif  // DGSSORT_SRC_THREADS_BUFFERQUEUE_H
