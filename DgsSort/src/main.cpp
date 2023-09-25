/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 James Till Matta
**
********************************************************************************
*******************************************************************************/
// includes for C system headers
// includes for C++ system headers
#include<iostream>
#include<cstdint>
#include<cmath>
#include<thread>
// includes from other libraries
#include<zlib.h>
// includes from DgsSort
#include"Utility/CmdLine.h"
#include"DgsSortBuildConfig.h"
#include"Output/HistWriter.h"
#include"Reader/DgsReader.h"
#include"Reader/Calibrator.h"
#include"Threads/BufferQueue.h"
#include"Threads/ReaderThread.h"

static const int64_t TimeStampOverlap = 100;
static const uint64_t ReportIntMult = 8;
static const uint64_t MegaMultiplier = 1048576;
static const uint64_t ReportInterval = ReportIntMult * MegaMultiplier;
static const uint64_t ReportIntMask = ReportInterval - 1;

int main(int argc, char* argv[])
{
    Utility::CmdLineOpts clo{};
    if(!Utility::parseCommandLine(argc, argv, clo))
    {
        return -1;
    }

    // ok, good commandline parameters, start thing up

    std::cout << "\nDgsSort\nDigital GammaSphere Sorter"
              << "\n     Version: " << DGSSORT_VERSION
              << "\n  Build Type: " << DGSSORT_BUILD
              << "\n  Git Commit: " << DGSSORT_COMMIT_SHA << std::endl;// NOLINT I want the forced flush

    //tell the user what our command line options are
    std::cout << "\nRaw DigitalGammaSphere File Name: " << clo.dgsFileName
              << "\n           Root Output File Name: " << clo.rootFileName
              << "\n Root Output File Opening Option: " << clo.rootFileOptions
              << "\n     Calibration Input File Name: " << clo.calFileName
              << "\n    Residual Nucleus Recoil Beta: " << clo.recoilBeta
              << "\n" << std::flush;

    // open the input file
//    gzFile inputFile = gzopen(clo.dgsFileName.c_str(), "rb");
    Threading::BufferQueue<Threading::BufferBlock, Threading::BlocksInFlight> queue;
    Threading::ReaderThread reader(clo.dgsFileName, queue);

    // open the calibration file
    Reader::DGS::Calibrator cal(clo.calFileName, clo.recoilBeta);

    // create our output system, do it with new so that we can delete it (forcing the output) before
    // we output "Done!" at the end, (otherwise the destructor would always go off right before return 0;)
    Output::HistWriter* writer = new Output::HistWriter(clo.rootFileName, clo.rootFileOptions);

    // timestamp holders to build our coincidences
    // thre previous author used uint64_t (well the equivalent) but then you are constantly converting
    // and if the timestampes are every 10ns then even int64_t takes you out ~2922 years
    int64_t tsEarly = 0;
    int64_t tsLate = 0;

    // structures to store data
    Reader::DGS::GebHeader header;
    Reader::DGS::DgsEventNew event;
    Reader::DGS::DgsTrace trace;
    Reader::DGS::CleanCoincidence clean;
    Reader::DGS::DirtyCoincidence dirty;
    Reader::DGS::AgnosticSinglesInfo ad;

    // some counters so we can tell the user what we are doing
    uint64_t gsEventCount = 0;
    uint64_t outputCount = 0;

    // start the reading thread
    std::thread readThread([&reader](){reader.run();});

    Threading::BufferBlock* prev{nullptr};
    Threading::BufferBlock* curr{nullptr};
    // start getting buffers then checking to make sure that we don't need to copy a little data from a previous buffer
    // to ensure continuity
    curr = queue.getFull();
    while(curr != nullptr)
    {
        // if curr is not nullptr then we have more data, ensure continuity
        if(prev != nullptr)
        {
            uint64_t prevEnd = (Threading::BufferOversize + prev->used);
            if(prev->startInd != prevEnd)
            {
                uint64_t remaining = (prevEnd - prev->startInd);
                if(remaining > Threading::BufferOversize)
                {
                    //uh oh
                    std::cout << "Remaining Buffer is Larger than BufferOversize... uh oh\n" << std::flush;
                    throw std::exception();
                }
                curr->startInd = Threading::BufferOversize - remaining;
                std::memcpy(curr->buffer+curr->startInd, prev->buffer+prev->startInd, remaining);
            }
            queue.pushEmpty(prev);
            prev = nullptr;
        }
        // at this point we have a good block of data, start processing the buffer
        uint64_t lastInd = (Threading::BufferOversize + curr->used);
        while(curr->startInd + sizeof(Reader::DGS::GebHeader) <= lastInd)
        {
            Reader::DGS::parseEvHdr(header, curr->buffer + curr->startInd);
            uint64_t nextStartInd = (curr->startInd + sizeof(Reader::DGS::GebHeader) + header.length);
            if(nextStartInd > lastInd)
            {
                // there is not enough in the buffer to hold the next event, break this loop
                break;
            }
            // as long as we get a new event, keep looping
            if(header.type == Reader::DGS::GammasphereType)
            {
                // for gammasphere events, do something for all others ignore
                // read the event from the buffer, skip processing the trace
                Reader::DGS::getEv(curr->buffer + curr->startInd + sizeof(Reader::DGS::GebHeader),
                                   event, trace, false);
                // now examine timestamps
                tsEarly = (header.timestamp < tsEarly) ? header.timestamp : tsEarly;
                tsLate = (header.timestamp > tsLate) ? header.timestamp : tsLate;
                // shaofei used llabs, but the one provided by C++ will adapt itself to its arguments, so use it
                if(std::abs(tsLate - tsEarly) < TimeStampOverlap)
                {
                    // this event is within our current pulse window, so add it to the "dirty event"
                    // all events are "presumed dirty" and the clean separated from the dirty at a later step
                    // this was somewhat confusing in the previous author's code so I am writing it here for sure
                    Reader::DGS::extractDirtyCoinsFromEvt(dirty, event, cal, ad);
                    // now increment the singles info that is agnostic to clean vs dirty
                    writer->incrementAgnosticSinglesData(ad);
                }
                else
                {
                    // if we are here the new event does not match our coincidence window, process what we have
                    // then clear things and start again
                    if(dirty.nDirty >= 1)
                    {
                        // extract any clean events
                        Reader::DGS::extractCleanCoinsFromDirty(dirty, clean);
                        // now increment the dirty histograms
                        writer->incrementDirtyEvents(dirty);
                    }
                    // now see if we extracted any clean events
                    if(clean.nClean > 1)
                    {
                        // we did! increment histograms
                        writer->incrementCleanEvents(clean);
                    }
                    tsEarly = header.timestamp;
                    tsLate = header.timestamp;
                    dirty.nDirty = 0;
                    Reader::DGS::extractDirtyCoinsFromEvt(dirty, event, cal, ad);

                }
                ++gsEventCount;
                // the previous author use modulus here. if you are working with powers of 2
                // bitwise mask is the same as modulus and does not invoke integer long division
                // integer long division is an _expensive_ instruction, taking 40 to 60 cycles,
                // and, unlike other almost all other instructions, not pipelined, so you cannot start
                // another integer long division until the last one is done, unlike pipelined instructions
                // where you can start a new one every cycle even if the old one has not completed yet.
                if((gsEventCount & ReportIntMask) == 0)
                {
                    ++outputCount;
                    std::cout << "Processed: " << gsEventCount << " - ("
                              << ReportIntMult * outputCount << " M) Events\n" << std::flush;
                }
            }
            curr->startInd = nextStartInd;
        }


        // at this point there is less than 1 header + event in the buffer, prepare to get another
        if(curr->startInd != (Threading::BufferOversize + curr->used))
        {
            prev = curr;
            curr = nullptr;
        }
        else
        {
            queue.pushEmpty(curr);
            prev = nullptr;
            curr = nullptr;
        }
        curr = queue.getFull();
    }

    // if we are here and prev is not nullptr... something's up tell the user
    if(prev != nullptr)
    {
        uint64_t remaining = (Threading::BufferOversize + prev->used - prev->startInd);
        std::cout << "Leaving event loop with: " << remaining << " bytes...?\n" << std::flush;
    }

    //join the shut down reader thread to keep things happy
    readThread.join();

    // delete the writer (forcing the writing of everything to the file)
    delete writer;
    // output some information then tell the world we are done
    std::cout << "Processed: " << gsEventCount << " - ("
              << ReportIntMult * outputCount + static_cast<double>(gsEventCount - (outputCount * ReportInterval))/MegaMultiplier
              <<" ) Events\n" << std::flush;
    std::cout << "Done!\n" << std::flush;
    // the destructor for the input file will trip on return and close the file
    return 0;
}
