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
#include"Reader/DgsReader.h"

namespace Output
{

//TODO write constants with binning parameters for histograms for faster bin lookup

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
    void incrementCleanEvents(CleanCoin const& evt);
    void incrementDirtyEvents(DirtyCoin const& evt);

private:
    TFile* outFile;
    TH2I* ringMatrices[17]{nullptr};
    TH2I* gRate{nullptr};
    TH2I* dtGe_BGO{nullptr};
    TH2I* dtGe_Ge{nullptr};
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
