/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 Oak Ridge National Laboratory
**
********************************************************************************
*******************************************************************************/
// includes for C system headers
// includes for C++ system headers
#include<iostream>
#include<cstdint>
#include<cmath>
// includes from other libraries
#include<zlib.h>
// includes from DgsSort
#include"Utility/CmdLine.h"
#include"DgsSortBuildConfig.h"
#include"Output/HistWriter.h"
#include"Utility/Misc.h"
#include"Reader/DgsReader.h"


static const int64_t TimeStampOverlap = 100;

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
    gzFile inputFile = gzopen(clo.dgsFileName.c_str(), "rb");

    Output::HistWriter writer(clo.rootFileName, clo.rootFileOptions);

    uint64_t bufferSize = Utility::CacheLineAlignment; // initial size of 64 bytes
    uint8_t* eventBuffer = Utility::cacheAlignedAlloc(bufferSize);

    Reader::DGS::GebHeader header;
    Reader::DGS::DgsEventNew event;
    Reader::DGS::DgsTrace trace;

    // torben or shaofei used uint64_t (well the equivalent) but then you are constantly converting
    // and if the timestampes are every 10ns then even int64_t takes you out ~2922 years
    int64_t tsEarly = 0;
    int64_t tsLate = 0;

    Reader::DGS::CleanCoincidence cleanEvt;
    Reader::DGS::DirtyCoincidence dirtyEvt;
    while(Reader::DGS::getEvBuf(inputFile, clo.dgsFileName, header, eventBuffer, bufferSize))
    {
        // as long as we get a new event, keep looping
        if(header.type == Reader::DGS::GammasphereType)
        {
            // for gammasphere events, do something for all others ignore
            // read the event from the buffer, skip processing the trace
            Reader::DGS::getEv(eventBuffer, event, trace, false);
            // now examine timestamps
            tsEarly = (header.timestamp < tsEarly) ? header.timestamp : tsEarly;
            tsLate = (header.timestamp > tsLate) ? header.timestamp : tsLate;
            // shaofei used llabs, but the one provided by C++ will adapt itself to its arguments, so use it
            if(std::abs(tsLate - tsEarly) < TimeStampOverlap)
            {
                // this event is within our current time coincidence window, so add it to the "dirty event"
                // all events are "presumed dirty" and the clean separated from the dirty at a later step
                // this was somewhat confusing until I figured it out

            }
            else
            {

            }
        }

    }

    return 0;
}
