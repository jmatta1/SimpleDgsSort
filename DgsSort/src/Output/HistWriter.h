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
#ifndef DGSSORT_SRC_OUTPUT_HISTWRITER_H
#define DGSSORT_SRC_OUTPUT_HISTWRITER_H

// includes for C system headers
// includes for C++ system headers
#include<string>
// includes from other libraries
#include<TFile.h>
#include<TH2I.h>
#include<TH3I.h>
// includes from DgsSort
#include"BinStructs.h"
#include"Reader/DgsReader.h"

namespace Output
{

using Params::DGS::NumGsGeDet;
using Params::DGS::NumRings;

static constexpr AxisParams RingMatBoth{4096, 0.0, 4096.0};
static constexpr BinCalc2D RingMatBin{RingMatBoth, RingMatBoth};

static constexpr AxisParams GRateX{7200, 0.0, 7200.0};
static constexpr AxisParams GRateY{NumGsGeDet, -0.5, NumGsGeDet - 0.5};
static constexpr BinCalc2dIndexY GRateBin{GRateX};

static constexpr AxisParams DtGeBgoX{2048, -1024.0, 1024.0};
static constexpr AxisParams DtGeBgoY{NumGsGeDet, -0.5, NumGsGeDet - 0.5};
static constexpr BinCalc2dIndexY DtGeBgoBin{DtGeBgoX};

static constexpr AxisParams DtGeGeX{2048, -1024.0, 1024.0};
static constexpr AxisParams DtGeGeY{NumGsGeDet, -0.5, NumGsGeDet - 0.5};
static constexpr BinCalc2dIndexY DtGeGeBin{DtGeGeX};

static constexpr AxisParams GgCleanBoth{4096, 0.0, 4096.0};
static constexpr BinCalc2D GgCleanBin{GgCleanBoth, GgCleanBoth};

static constexpr AxisParams GCleanX{8192, 0, 8192};
static constexpr AxisParams GCleanY{NumGsGeDet, -0.5, NumGsGeDet - 0.5};
static constexpr BinCalc2dIndexY GCleanBin{GCleanX};

static constexpr AxisParams GDirtyX{8192, 0, 8192};
static constexpr AxisParams GDirtyY{NumGsGeDet, -0.5, NumGsGeDet - 0.5};
static constexpr BinCalc2dIndexY GDirtyBin{GDirtyX};

static constexpr AxisParams GgDirtyBoth{4096, 0, 4096};
static constexpr BinCalc2D GgDirtyBin{GgDirtyBoth, GgDirtyBoth};

static constexpr AxisParams GRingX{4096, 0, 4096};
static constexpr AxisParams GRingY{NumRings, -0.5, NumRings - 0.5};
static constexpr BinCalc2dIndexY GRingBin{GRingX};

static constexpr AxisParams GdtX{4096, 0, 4096};
static constexpr AxisParams GdtY{1000, 0, 1000};
static constexpr BinCalc2D GdtBin{GdtX, GdtY};

static constexpr AxisParams GbaseX{7200, 0, 7200};
static constexpr AxisParams GbaseY{1300, -100, 1200};
static constexpr BinCalc2D GbaseBin{GbaseX, GbaseY};

static constexpr AxisParams GringDopX{4096, 0, 4096};
static constexpr AxisParams GringDopY{NumRings, -0.5, NumRings - 0.5};
static constexpr BinCalc2dIndexY GringDopBin{GringDopX};

static constexpr AxisParams GgRingXY{2048, 0, 4096};
static constexpr AxisParams GgRingZ{NumRings, -0.5, NumRings - 0.5};
static constexpr BinCalc3dIndexZ GgRingBin{GgRingXY, GgRingXY};

/*!
* @brief
*/
class HistWriter
{
    using CleanCoin = Reader::DGS::CleanCoincidence;
    using DirtyCoin = Reader::DGS::DirtyCoincidence;
    using AgnosticData = Reader::DGS::AgnosticSinglesInfo;
public:
    HistWriter(std::string const& outFileName, std::string const& fileOption);
    ~HistWriter();

    // increments the data that is agnostic to cleanliness or dirtiness
    void incrementAgnosticSinglesData(AgnosticData const& ad);
    // increments the "clean event only histograms
    void incrementCleanEvents(CleanCoin const& cc);
    // increments the dirty event histograms
    void incrementDirtyEvents(DirtyCoin const& dc);

private:
    TFile* outFile;
    TH2I* ringMatrices[17]{nullptr};
    TH2I* gRate{nullptr};
    TH2I* dtGeBGO{nullptr};
    TH2I* dtGeGe{nullptr};
    TH2I* ggClean{nullptr};
    TH2I* gClean{nullptr};
    TH2I* gDirty{nullptr};
    TH2I* ggDirty{nullptr};
    TH2I* gring{nullptr};
    TH2I* gdt{nullptr};
    TH2I* gbase{nullptr};
    TH2I* gringdop{nullptr};
    TH3I* ggrClean{nullptr};
};

} // namespace Output

#endif  // DGSSORT_SRC_OUTPUT_HISTWRITER_H
