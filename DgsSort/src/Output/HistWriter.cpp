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
                                       RingMatBoth.nb, RingMatBoth.lo, RingMatBoth.hi,
                                       RingMatBoth.nb, RingMatBoth.lo, RingMatBoth.hi);
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
                         GRateX.nb, GRateX.lo, GRateX.hi,
                         GRateY.nb, GRateY.lo, GRateY.hi);
    }

    dtGeBGO = outFile->Get<TH2I>("dtGe_BGO");
    if(gRate == nullptr)
    {
        dtGeBGO = new TH2I("dtGe_BGO", "Ge-BGO & BGO-BGO Gamma Coincidence Times vs Det Number",
                            DtGeBgoX.nb, DtGeBgoX.lo, DtGeBgoX.hi,
                            DtGeBgoY.nb, DtGeBgoY.lo, DtGeBgoY.hi);
    }

    dtGeGe = outFile->Get<TH2I>("dtGe_Ge");
    if(gRate == nullptr)
    {
        dtGeGe = new TH2I("dtGe_Ge", "Ge-Ge Gamma Coincidence Times vs Det Number",
                           DtGeGeX.nb, DtGeGeX.lo, DtGeGeX.hi,
                           DtGeGeY.nb, DtGeGeY.lo, DtGeGeY.hi);
    }

    ggClean = outFile->Get<TH2I>("ggClean");
    if(gRate == nullptr)
    {
        ggClean = new TH2I("ggClean", "Clean Gamma-Gamma Coincidence Matrix",
                           GgCleanBoth.nb, GgCleanBoth.lo, GgCleanBoth.hi,
                           GgCleanBoth.nb, GgCleanBoth.lo, GgCleanBoth.hi);
    }

    gClean = outFile->Get<TH2I>("gClean");
    if(gRate == nullptr)
    {
        gClean = new TH2I("gClean", "Per Detector Clean Gamma Spectra",
                          GCleanX.nb, GCleanX.lo, GCleanX.hi,
                          GCleanY.nb, GCleanY.lo, GCleanY.hi);
    }

    gDirty = outFile->Get<TH2I>("gDirty");
    if(gRate == nullptr)
    {
        gDirty = new TH2I("gDirty", "Per Detector Dirty Gamma Spectra",
                          GDirtyX.nb, GDirtyX.lo, GDirtyX.hi,
                          GDirtyY.nb, GDirtyY.lo, GDirtyY.hi);
    }

    ggDirty = outFile->Get<TH2I>("ggDirty");
    if(gRate == nullptr)
    {
        ggDirty = new TH2I("ggDirty", "Dirty Gamma-Gamma Coincidence Matrix",
                           GgDirtyBoth.nb, GgDirtyBoth.lo, GgDirtyBoth.hi,
                           GgDirtyBoth.nb, GgDirtyBoth.lo, GgDirtyBoth.hi);
    }

    gring = outFile->Get<TH2I>("gring");
    if(gRate == nullptr)
    {
        gring = new TH2I("gring", "Per Ring (Y) Un-Doppler Corrected Clean + Dirty Energy (X)",
                         GRingX.nb, GRingX.lo, GRingX.hi,
                         GRingY.nb, GRingY.lo, GRingY.hi);
    }

    gdt = outFile->Get<TH2I>("gdt");
    if(gRate == nullptr)
    {
        gdt = new TH2I("gdt", "Delta Event Discriminator Time (Y) vs Gamma Energy (X)",
                       GdtX.nb, GdtX.lo, GdtX.hi,
                       GdtY.nb, GdtY.lo, GdtY.hi);
    }

    gbase = outFile->Get<TH2I>("gbase");
    if(gRate == nullptr)
    {
        gbase = new TH2I("gbase", "Baseline Correction Scaled by ID(Y-Axis) vs. Event Time From File Start (X-Axis)",
                         GbaseX.nb, GbaseX.lo, GbaseX.hi,
                         GbaseY.nb, GbaseY.lo, GbaseY.hi);
    }

    gringdop = outFile->Get<TH2I>("gringdop");
    if(gRate == nullptr)
    {
        gringdop = new TH2I("gringdop", "Doppler Corrected Per Ring Clean Energy Spectra",
                            GringDopX.nb, GringDopX.lo, GringDopX.hi,
                            GringDopY.nb, GringDopY.lo, GringDopY.hi);
    }

    ggrClean = outFile->Get<TH3I>("ggrClean");
    if(gRate == nullptr)
    {
        ggrClean = new TH3I("ggrClean", "Clean Ring Number (z) vs Ring Energy (y) vs All Energy (x)",
                            GgRingXY.nb, GgRingXY.lo, GgRingXY.hi,
                            GgRingXY.nb, GgRingXY.lo, GgRingXY.hi,
                            GgRingZ.nb, GgRingZ.lo, GgRingZ.hi);
    }
}

HistWriter::~HistWriter()
{
    using Params::DGS::NumRings;
    // first save all the histograms currently in memory
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
    delete dtGeGe;
    delete dtGeBGO;
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
    // I know that using Fill is tempting. _BUT_ Fill uses binary search to find the right axis
    // bin for a given axis value. even for linear axes where you can compute it and not bounce
    // around an array slowing everything down so you are taking the number of operations from
    // Order(Log_2(BinCount)) to Order(1) (a constant), and the operations for Order(Log_2(BinCount))
    // are expensive because they are jumping around in memory semi-randomly

    // these fills all come from IncrementDirtyEvent in the old code
    int bin = GRateBin(ad.eventTime, ad.channel);
    gRate->AddBinContent(bin, 1);

    bin = GRingBin(ad.uncorEn, ad.ringId);
    gring->AddBinContent(bin, 1);

    bin = GdtBin(ad.corrEn, ad.deltaDiscEvTime);
    gdt->AddBinContent(bin, 1);

    bin = GbaseBin(ad.eventTime, ad.baseParam);
    gbase->AddBinContent(bin, 1);
    // this was moved here from ProcessDirtyEvent under the theory that
    // since it only triggers for germanium anyways, this is equivalent
    bin = GDirtyBin(ad.corrEn, ad.channel);
    gDirty->AddBinContent(bin, 1);
}

void HistWriter::incrementCleanEvents(CleanCoin const& cc)
{
    for(unsigned i=0; i<cc.nClean; ++i)
    {
        int bin = GCleanBin(cc.e[i], cc.id[i]);
        gClean->AddBinContent(bin, 1);
        int detIring = Params::DGS::DetRings[cc.id[i]];
        bin = GringDopBin(cc.e[i], detIring);
        gringdop->AddBinContent(bin, 1);

        for(unsigned j=i+1; j<cc.nClean; ++i)
        {
            bin = GgCleanBin(cc.e[i], cc.e[j]);
            ggClean->AddBinContent(bin, 1);
            bin = GgCleanBin(cc.e[j], cc.e[i]);
            ggClean->AddBinContent(bin, 1);

            int detJring = Params::DGS::DetRings[cc.id[i]];

            bin = RingMatBin(cc.e[j], cc.e[i]);
            ringMatrices[detIring]->AddBinContent(bin, 1);
            bin = RingMatBin(cc.e[i], cc.e[j]);
            ringMatrices[detJring]->AddBinContent(bin, 1);

            bin = GgRingBin(cc.e[j], cc.e[i], detIring);
            ggrClean->AddBinContent(bin, 1);
            bin = GgRingBin(cc.e[i], cc.e[j], detJring);
            ggrClean->AddBinContent(bin, 1);
        }
    }
}

void HistWriter::incrementDirtyEvents(DirtyCoin const& dc)
{
    int bin{0};
    for(unsigned i=0; i<dc.nDirty; ++i)
    {
        if(dc.gamma[i].GE)
        {
            bin = GDirtyBin(dc.gamma[i].e, dc.gamma[i].id);
            gDirty->AddBinContent(bin, 1);
        }
        for(unsigned j=i+1; j<dc.nDirty; ++j)
        {
            uint64_t dt = dc.gamma[i].t - dc.gamma[j].t;
            if(dc.gamma[i].GE && dc.gamma[j].GE)
            {
                bin = DtGeGeBin(dt, dc.gamma[i].id);
                dtGeGe->AddBinContent(bin, 1);
                bin = DtGeGeBin(dt, dc.gamma[j].id);
                dtGeGe->AddBinContent(bin, 1);
                if(((dt < 0) ? -dt : dt) < Params::DGS::PromptCoincidentWindowTime)
                {
                    bin = GgDirtyBin(dc.gamma[i].e, dc.gamma[j].e);
                    ggDirty->AddBinContent(bin, 1);
                    bin = GgDirtyBin(dc.gamma[j].e, dc.gamma[i].e);
                    ggDirty->AddBinContent(bin, 1);
                }
            }
            if(dc.gamma[i].id == dc.gamma[j].id)
            {
                bin = DtGeBgoBin(dt, dc.gamma[i].id);
                dtGeBGO->AddBinContent(bin, 1);
            }
        }
    }
}



} // namespace Output

