/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 ??
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_READER_DGSSTRUCTS_H
#define DGSSORT_SRC_READER_DGSSTRUCTS_H

// includes for C system headers
// includes for C++ system headers
#include<cstdint>
// includes from other libraries
// includes from DgsSort
#include"Parameters/DgsParameters.h"

/*****************************************************************************************************************************
 * This is sort of a translation of DGSCHICOjames.h that Daniel sent me.
 * Matta Changes:
 * 1. Get rid of the `typedef struct` nonsense, not even C99 requires that crap, it's a holdover from _ancient_ C
 * 2. Use the types defined in cstdint / stdint.h instead of unweildy crap like unsigned long long int
 * 3. Change the struct names to not be all caps, ALLCAPS is for macros
 * 4. Remove the #define statements for constants. Sometimes macros are needed but they should be avoid if possible
 *      static const is a much cleaner constant definition
 * 5. Give all the structures 0 initializations
 *
 * Changes James _Wanted_ to make but dares not because he thinks Torben wrote some of this and he knows what Torben does in his code
 * 1. Rearrange all the structs from largest members to smallest (this almost always allows the compiler to remove padding
 *      in the middle of the struct, only the end padding remains). I didn't do it because I know Torben tends to read structs
 *      directly from files, which works if the member order, intra and inter struct padding, endianness etc etc stay the same
 *      Since he does not carefully deserialize things the way I do if I change the ordering that breaks. He claims it's faster
 *      I claim that I have written codes that read and process GS data faster than anything I have seen from him despite my more
 *      careful deserialization process that renders things platform independent.
 *
 * Repeat After Me:
 * MACROS ARE EVIL, AVOID UNLESS YOU HAVE NO CHOICE!!!!!
 ****************************************************************************************************************************/

namespace Reader::DGS
{

static const uint32_t MaxDgsNum = 80;
static const uint32_t MaxLaBrNum = 25;
static const uint32_t MaxMbNum = 16;
static const uint32_t MaxDssdNum = 16;
static const uint32_t MbLen = 102;

/* GT event */
/* __contains both raw and */
/* __processed quantities! */


struct DgsEvent
{
    uint16_t chan_id{0};
    uint16_t board_id{0};
    uint64_t LEDts{0};
    int32_t  PreEnergy{0};
    int32_t  PostEnergy{0};
    int32_t  BaseSample{0};
    uint16_t status{0};
};

struct DgsEventNew
{
    uint16_t chan_id{0};
    uint16_t board_id{0};
    uint16_t geo_addr{0};
    uint16_t event_length{0};

    uint64_t event_timestamp{0};
    uint64_t last_disc_timestamp{0};
    uint64_t peak_timestamp{0};

    uint16_t write_flag{0};
    uint16_t timestamp_match_flag{0};
    uint16_t external_disc_flag{0};
    uint16_t cfd_valid_flag{0};
    uint16_t pileup_only_flag{0};
    uint16_t offset_flag{0};
    uint16_t sync_error_flag{0};
    uint16_t general_error_flag{0};

    uint16_t peak_valid_flag{0};

    uint16_t pileup_flag{0};

    int32_t  sampled_baseline{0};
    int16_t  cfd_sample_0{0};
    int16_t  cfd_sample_1{0};
    int16_t  cfd_sample_2{0};
    int32_t  pre_rise_energy{0};
    int32_t  post_rise_energy{0};

    uint16_t m1_begin_sample{0};
    uint16_t m1_end_sample{0};
    uint16_t m2_begin_sample{0};
    uint16_t m2_end_sample{0};
    uint16_t m3_begin_sample{0};
    uint16_t m3_end_sample{0};
    uint16_t peak_sample{0};
    uint16_t base_sample{0};
};

struct DgsTrace
{
    uint16_t Len{0};
    int16_t  trace[1024]{0};//even though this is an array it should be zero initialized by this
};

struct ChicoEvent
{
    uint64_t LEDts{0};
    uint16_t cathode_tdc_num{0};
    int32_t  cathode_tdc_val[128]{0};
    uint16_t cathode_tdc_ch[128]{0};
    uint16_t anode_tdc_num{0};
    int32_t  anode_tdc_val[128]{0};
    uint16_t anode_tdc_ch[128]{0};
    uint16_t anode_qdc_num{0};
    int32_t  anode_qdc_val[32]{0};
    uint16_t anode_qdc_ch[32]{0};
    uint16_t status{0};
    int32_t  RF{0};
};

struct Particle
{
    int32_t  id{0};
    uint32_t t{0};
    float    m{0.0f};
    float    fthetaL{0.0f};
    float    fphiL{0.0f};
    float    fthetaR{0.0f};
    float    fphiR{0.0f};
    int32_t  eL{0};
    int32_t  eR{0};
    int32_t  rf{0};
};

struct Gamma
{
    uint64_t t{0};
    float    e{0.0f};
    uint16_t id{0};
    bool     BGO{0};
    bool     GE{0};
    bool     Compton{0};
};

struct DgsBuff
{
    unsigned int* DGSEventBuf[20]{0};
};

struct DgsBase
{
    float    base_sum[Params::DGS::NumGsGeDet + 1]{0.0f};
    float    n_base[Params::DGS::NumGsGeDet + 1]{0.0f};
};

struct GebHeader
{
    uint32_t type{0};
    int32_t  length{0}; /* length of payload following the header, in bytes */
    uint64_t timestamp{0};
};

struct DirtyCoincidence
{
    int32_t  nDirty{0};
    Gamma    gamma[MaxDgsNum]{0};
};

struct CleanCoincidence
{
    int32_t  nClean{0};
    double   tPrompt{0.0};
    uint64_t t[MaxDgsNum]{0};
    uint16_t id[MaxDgsNum]{0};
    float    e[MaxDgsNum]{0.0f};
};

struct Fatima
{
    uint16_t id{0};
    uint16_t e{0};
    uint32_t ts{0};
    uint32_t t{0};
};

struct FatimaEvent
{
    int32_t  nLaBr{0};
    uint64_t dgs_ts{0};
    Fatima   fatima[MaxLaBrNum];
    uint16_t status{0};
};


struct DgsFatimaEvent
{
    FatimaEvent      fatimaEvent;
    DirtyCoincidence DirtyEvent;
};

struct FpRaw
{
    uint16_t id{0};
    int32_t  e{0};
    uint64_t ts{0};
};

struct MbRaw
{
    uint16_t id{0};
    int32_t  e{0};
    uint64_t ts{0};
    int16_t  trace[MbLen]{0};
};

struct DssdRaw
{
    uint16_t id{0};
    uint16_t type{0};   //DE RING:1;DE SECTOR:2;E RING:3;E SECTOR:4
    float    e{0.0f};
    float    angle{0.0f};
    uint64_t ts{0};
};

struct DfmaEvent
{
    uint16_t nFP{0};
    uint16_t nMB{0};
    uint16_t nDSSD{0};
    FpRaw    FP[10];
    MbRaw    MB[MaxMbNum];
    DssdRaw  DSSD[MaxDssdNum];
};

struct FocalPlane
{
    float    x{0.0f};
    float    y{0.0f};
    float    tgppac{0.0f};
    float    ppacde{0.0f};
    float    de1{0.0f};
    float    de2{0.0f};
    float    etot{0.0f};
    uint64_t ts{0};
    bool     FPTrue{false};
};

struct MbEvent
{
    uint16_t nMB{0};
    uint16_t id[MaxMbNum]{0};
    uint64_t ts[MaxMbNum]{0};
    float    etot[MaxMbNum]{0.0f};
    float    etail[MaxMbNum]{0.0f};
};


struct DssdEvent
{
    uint16_t nDSSD{0};
    uint64_t ts[MaxDssdNum]{0};
    float    e[MaxDssdNum]{0.0f};
    uint16_t id[MaxDssdNum]{0};
    uint16_t type[MaxDssdNum]{0};
};

} // namespace Reader::DGS

#endif  // DGSSORT_SRC_READER_DGSSTRUCTS_H
