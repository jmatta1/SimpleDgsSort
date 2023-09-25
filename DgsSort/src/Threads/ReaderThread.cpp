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
#include"ReaderThread.h"
// includes for C system headers
// includes for C++ system headers
#include<iostream>
// includes from other libraries
// includes from DgsSort

namespace Threading
{

ReaderThread::ReaderThread(const std::string & inFileName, QueueType & queue)
    :outQueue(queue)
{
    inputFile = gzopen(inFileName.c_str(), "rb");
    gzbuffer(inputFile, GzipInternalBufferSize);
}

ReaderThread::~ReaderThread()
{
    gzclose(inputFile);
}

void ReaderThread::run()
{
    std::cout << "Reader Thread Starting\n" << std::flush;
    BufferBlock* blk = outQueue.getEmpty();
    blk->reset();
    blk->used = gzread(inputFile, blk->buffer+blk->startInd, ReadSizeInBytes);
    while(blk->used == ReadSizeInBytes)
    {
        // got a full read, woot
        blk->used = ReadSizeInBytes;
        outQueue.pushFull(blk);
        blk = outQueue.getEmpty();
        blk->reset();
        blk->used = gzread(inputFile, blk->buffer+blk->startInd, ReadSizeInBytes);
    }
    if(blk->used > 0)
    {
        outQueue.pushFull(blk);
    }
    else
    {
        outQueue.pushEmpty(blk);
    }
    // send the done signal
    outQueue.pushFull(nullptr);
    std::string message = "Reader Thread Quiting\n";
    if(gzeof(inputFile))
    {
        message += "End of file Reached\n";
    }
    else
    {
        message += "File read error\n";
    }
    std::cout << message << std::flush;
}


} // namespace Threading

