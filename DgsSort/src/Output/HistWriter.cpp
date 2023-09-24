/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 24 Sep, 2023
**
** @copyright Copyright (C) 2023 ??
**
********************************************************************************
*******************************************************************************/
#include"HistWriter.h"
// includes for C system headers
// includes for C++ system headers
#include<cstdio>
// includes from other libraries
// includes from DgsSort
#include"Parameters/DgsParameters.h"

namespace Output
{

HistWriter::HistWriter(const std::string & outFileName, std::string const& fileOption)
{
    using Params::DGS::NumGsGeDet;
    using Params::DGS::NumRings;

    char buffer[128];
    outFile = TFile::Open(outFileName.c_str(), fileOption.c_str());
    //now set up the histograms
    for(unsigned i = 0; i<NumRings; ++i)
    {
        std::snprintf(buffer, 128, "ringmat%2.2i", i);
        ringMatrices[i] = outFile->Get<TH2F>(buffer);
        if(ringMatrices[i] == nullptr)
        {
            ringMatrices[i] = new TH2F(buffer, buffer, 4096, 0, 4096, 4096, 0, 4096);
        }
    }

    // I am setting axes that are meant for integer values like detector number and ring number to go from
    // -0.5 to DetCount (or RingCount) - 0.5 so that the actual "value" is the center of the bin.
    // i.e. detector 0 winds up in the bin going from -0.5 to 0.5 which is centered at 0, detector 109 winds
    // up in the top-most bin going from 108.5 to 109.5 centered at 109, and all the other bins act the same
    gRate = outFile->Get<TH2F>("gRate");
    if(gRate == nullptr)
    {
        gRate = new TH2F("gRate", "gRate", 7200, 0, 7200, NumGsGeDet, -0.5, NumGsGeDet-0.5);
    }

    dtGe_BGO = outFile->Get<TH2F>("dtGe_BGO");
    if(gRate == nullptr)
    {
        dtGe_BGO = new TH2F("dtGe_BGO", "dtGe_BGO", 2048, -1024, 1024, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    dtGe_Ge = outFile->Get<TH2F>("dtGe_Ge");
    if(gRate == nullptr)
    {
        dtGe_Ge = new TH2F("dtGe_Ge", "dtGe_Ge", 2048, -1024, 1024, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    ggClean = outFile->Get<TH2F>("ggClean");
    if(gRate == nullptr)
    {
        ggClean = new TH2F("ggClean", "ggClean", 4096, 0, 4096, 4096, 0, 4096);
    }

    gClean = outFile->Get<TH2F>("gClean");
    if(gRate == nullptr)
    {
        gClean = new TH2F("gClean", "gClean", 8192, 0, 8192, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    gDirty = outFile->Get<TH2F>("gDirty");
    if(gRate == nullptr)
    {
        gDirty = new TH2F("gDirty", "gDirty", 8192, 0, 8192, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    ggDirty = outFile->Get<TH2F>("ggDirty");
    if(gRate == nullptr)
    {
        ggDirty = new TH2F("ggDirty", "ggDirty", 4096, 0, 4096, 4096, 0, 4096);
    }

    gring = outFile->Get<TH2F>("gring");
    if(gRate == nullptr)
    {
        gring = new TH2F("gring", "gring", 4096, 0, 4096, NumRings, -0.5, NumRings - 0.5);
    }

    gdt = outFile->Get<TH2F>("gdt");
    if(gRate == nullptr)
    {
        gdt = new TH2F("gdt", "gdt", 4096, 0, 4096, 1000, 0, 1000);
    }

    gbase = outFile->Get<TH2F>("gbase");
    if(gRate == nullptr)
    {
        gbase = new TH2F("gbase", "gbase", 3600, 0, 3600, 1300, -100, 1200);
    }

    gringdop = outFile->Get<TH2F>("gringdop");
    if(gRate == nullptr)
    {
        gringdop = new TH2F("gringdop", "gringdop", 4096, 0, 4096, NumRings, -0.5, NumRings - 0.5);
    }

    ggrClean = outFile->Get<TH3F>("ggrClean");
    if(gRate == nullptr)
    {
        ggrClean = new TH3F("ggrClean", "ggrClean", 2048, 0, 4096, 2048, 0, 4096, NumRings, -0.5, NumRings - 0.5);
    }
}

HistWriter::~HistWriter()
{
    using Params::DGS::NumRings;
    // first save all the histograms in memory
    outFile->Write(nullptr, TObject::kOverwrite);
    // make sure they're all on the disk
    outFile->Flush();
    // now delete all the histograms in reverse order of creation
    // technically the reverse order is not strictly necessary but it is
    // generally a good practice
    delete ggrClean;
    delete gringdop;
    delete gbase;
    delete gdt;
    delete gring;
    delete ggDirty;
    delete gDirty;
    delete gClean;
    delete ggClean;
    delete dtGe_Ge;
    delete dtGe_BGO;
    delete gRate;
    for(int i = NumRings-1; i > -1; --i)
    {
        delete ringMatrices[i];
    }
    // now close and delete the file
    outFile->Close();
    delete outFile;
}

void HistWriter::incrementCleanEvent(CleanCoin * evt)
{

}

void HistWriter::incrementDirtyEvent(DirtyCoin * evt)
{

}



} // namespace Output

