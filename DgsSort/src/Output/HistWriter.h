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
#ifndef DGSSORT_SRC_OUTPUT_HISTWRITER_H
#define DGSSORT_SRC_OUTPUT_HISTWRITER_H

// includes for C system headers
// includes for C++ system headers
#include<string>
// includes from other libraries
#include<TFile.h>
#include<TH1F.h>
#include<TH2F.h>
#include<TH3F.h>
// includes from DgsSort
#include"Reader/DgsReader.h"

namespace Output
{

/*!
* @brief
*/
class HistWriter
{
    using CleanCoin = Reader::DGS::CleanCoincidence;
    using DirtyCoin = Reader::DGS::DirtyCoincidence;
    using DgsEvent = Reader::DGS::DgsEventNew;
public:
    HistWriter(std::string const& outFileName, std::string const& fileOption);
    ~HistWriter();

    void incrementCleanEvent(CleanCoin * evt);
    void incrementDirtyEvent(DirtyCoin * evt);

private:
    TFile* outFile;
    TH2F* ringMatrices[17]{nullptr};
    TH2F* gRate{nullptr};
    TH2F* dtGe_BGO{nullptr};
    TH2F* dtGe_Ge{nullptr};
    TH2F* ggClean{nullptr};
    TH2F* gClean{nullptr};
    TH2F* gDirty{nullptr};
    TH2F* ggDirty{nullptr};
    TH2F* gring{nullptr};
    TH2F* gdt{nullptr};
    TH2F* gbase{nullptr};
    TH2F* gringdop{nullptr};
    TH3F* ggrClean{nullptr};
};

} // namespace Output

#endif  // DGSSORT_SRC_OUTPUT_HISTWRITER_H
