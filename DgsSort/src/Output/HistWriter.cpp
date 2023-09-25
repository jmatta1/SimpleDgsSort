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

    outFile = TFile::Open(outFileName.c_str(), fileOption.c_str());
    //now set up the histograms
    for(unsigned i = 0; i<NumRings; ++i)
    {
        char nameBuffer[128];
        char titleBuffer[256];
        std::snprintf(nameBuffer, 128, "ringmat%2.2i", i);
        std::snprintf(titleBuffer, 256, "Ring%2.2i Energy (Y) vs All Energy (X)", i);

        ringMatrices[i] = outFile->Get<TH2I>(nameBuffer);
        if(ringMatrices[i] == nullptr)
        {
            ringMatrices[i] = new TH2I(nameBuffer, titleBuffer,
                                       4096, 0, 4096, 4096, 0, 4096);
        }
    }

    // I am setting axes that are meant for integer values like detector number and ring number to go from
    // -0.5 to DetCount (or RingCount) - 0.5 so that the actual "value" is the center of the bin.
    // i.e. detector 0 winds up in the bin going from -0.5 to 0.5 which is centered at 0, detector 109 winds
    // up in the top-most bin going from 108.5 to 109.5 centered at 109, and all the other bins act the same
    gRate = outFile->Get<TH2I>("gRate");
    if(gRate == nullptr)
    {
        gRate = new TH2I("gRate", "Counts Per Detector (Y) Per Second From File Start (X)",
                         7200, 0, 7200, NumGsGeDet, -0.5, NumGsGeDet-0.5);
    }

    dtGe_BGO = outFile->Get<TH2I>("dtGe_BGO");
    if(gRate == nullptr)
    {
        dtGe_BGO = new TH2I("dtGe_BGO", "dtGe_BGO", 2048, -1024, 1024, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    dtGe_Ge = outFile->Get<TH2I>("dtGe_Ge");
    if(gRate == nullptr)
    {
        dtGe_Ge = new TH2I("dtGe_Ge", "dtGe_Ge", 2048, -1024, 1024, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    ggClean = outFile->Get<TH2I>("ggClean");
    if(gRate == nullptr)
    {
        ggClean = new TH2I("ggClean", "ggClean", 4096, 0, 4096, 4096, 0, 4096);
    }

    gClean = outFile->Get<TH2I>("gClean");
    if(gRate == nullptr)
    {
        gClean = new TH2I("gClean", "gClean", 8192, 0, 8192, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    gDirty = outFile->Get<TH2I>("gDirty");
    if(gRate == nullptr)
    {
        gDirty = new TH2I("gDirty", "gDirty", 8192, 0, 8192, NumGsGeDet, -0.5, NumGsGeDet - 0.5);
    }

    ggDirty = outFile->Get<TH2I>("ggDirty");
    if(gRate == nullptr)
    {
        ggDirty = new TH2I("ggDirty", "ggDirty", 4096, 0, 4096, 4096, 0, 4096);
    }

    gring = outFile->Get<TH2I>("gring");
    if(gRate == nullptr)
    {
        gring = new TH2I("gring", "Per Ring (Y) Un-Doppler Corrected Clean + Dirty Energy (X)",
                         4096, 0, 4096, NumRings, -0.5, NumRings - 0.5);
    }

    gdt = outFile->Get<TH2I>("gdt");
    if(gRate == nullptr)
    {
        gdt = new TH2I("gdt", "Delta Event Discriminator Time (Y) vs Gamma Energy (X)",
                       4096, 0, 4096, 1000, 0, 1000);
    }

    gbase = outFile->Get<TH2I>("gbase");
    if(gRate == nullptr)
    {
        gbase = new TH2I("gbase", "Baseline Correction Scaled by ID(Y-Axis) vs. Event Time From File Start (X-Axis)",
                         7200, 0, 7200, 1300, -100, 1200);
    }

    gringdop = outFile->Get<TH2I>("gringdop");
    if(gRate == nullptr)
    {
        gringdop = new TH2I("gringdop", "Doppler Corrected Per Ring Clean Energy Spectra",
                            4096, 0, 4096, NumRings, -0.5, NumRings - 0.5);
    }

    ggrClean = outFile->Get<TH3I>("ggrClean");
    if(gRate == nullptr)
    {
        ggrClean = new TH3I("ggrClean", "Clean Ring Number (z) vs Ring Energy (y) vs All Energy (x)",
                            2048, 0, 4096, 2048, 0, 4096, NumRings, -0.5, NumRings - 0.5);
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
void HistWriter::incrementAgnosticSinglesData(AgnosticData const& ad)
{
    //TODO write bin lookup functions rather than use full (constant binary searches _cringe_)
    // these fills all come from IncrementDirtyEvent in the old code
    gRate->Fill(ad.eventTime, ad.channel);
    gring->Fill(ad.uncorEn, ad.ringId);
    gdt->Fill(ad.corrEn, ad.deltaDiscEvTime);
    gbase->Fill(ad.eventTime, ad.baseParam);
    // this was moved here from ProcessDirtyEvent under the theory that
    // since it only triggers for germanium anyways, this is equivalent
    gDirty->Fill(ad.corrEn, ad.channel);
}

void HistWriter::incrementCleanEvents(CleanCoin const& evt)
{
    //TODO write increments with bin lookup
}

void HistWriter::incrementDirtyEvents(DirtyCoin const& evt)
{
    //TODO write increments with bin lookup
}



} // namespace Output

