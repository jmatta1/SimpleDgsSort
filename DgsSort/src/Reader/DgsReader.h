/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 Oak Ridge National Laboratory
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_READER_DGSSTRUCTS_H
#define DGSSORT_SRC_READER_DGSSTRUCTS_H

// includes for C system headers
// includes for C++ system headers
#include<cstdint>
#include<string>
// includes from other libraries
#include<zlib.h>
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
 * Also, Repeat After Me:
 * MACROS ARE EVIL, AVOID UNLESS YOU HAVE NO CHOICE!!!!!
 ****************************************************************************************************************************/
namespace Reader::DGS
{

// event type identifiers
static const uint32_t ChicoType       = 300;
static const uint32_t GammasphereType = 14;
static const uint32_t DfmaType        = 16;
static const uint32_t FatimaType      = 20;
static const uint32_t XarrayType      = 22;

// array size setters
static const uint32_t MaxDgsNum = 110;
static const uint32_t MaxLaBrNum = 25;
static const uint32_t MaxMbNum = 16;
static const uint32_t MaxDssdNum = 16;
static const uint32_t MbLen = 102;
static const uint32_t GebHdrLenBytes = 16;
static const uint32_t DgsTraceMaxLen = 1024;
// Torben's HDRLENINTS * 4 since I work with a byte buffer
static const uint32_t HdrLenBytes = 52;


// masks and shifts for reading raw binary
// dword 0
static const uint32_t ChanIdMask   = 0x0000000F;
static const uint32_t ChanIdShft   = 0;
static const uint32_t BoardIdMask  = 0x0000FFf0;
static const uint32_t BoardIdShft  = 4;
static const uint32_t EventLenMask = 0x07FF0000;
static const uint32_t EventLenShft = 16;
static const uint32_t GeoAddrMask  = 0xF8000000;
static const uint32_t GeoAddrShft  = 27;

// dword 1
static const uint32_t EvtTsLoMask = 0xFFFFFFFF;
static const uint32_t EvtTsLoShft = 0;

// dword 2
static const uint32_t EvtTsHiMask = 0x0000FFFF;
static const uint32_t EvtTsHiShft = 0;
static const uint32_t EvtTsHiUpShft = 32;

// dword 3
static const uint32_t TsMatchFlgMask = 0x00000080;
static const uint32_t TsMatchFlgShft = 7;
static const uint32_t ExtDiscFlgMask = 0x00000100;
static const uint32_t ExtDiscFlgShft = 8;
static const uint32_t PkValidFlgMask = 0x00000200;
static const uint32_t PkValidFlgShft = 9;
static const uint32_t OffsetFlgMask = 0x00000400;
static const uint32_t OffsetFlgShft = 10;
static const uint32_t CfdValidFlgMask = 0x00000800;
static const uint32_t CfdValidFlgShft = 11;
static const uint32_t SyncErrFlgMask = 0x00001000;
static const uint32_t SyncErrFlgShft = 12;
static const uint32_t GenErrFlgMask = 0x00002000;
static const uint32_t GenErrFlgShft = 13;
static const uint32_t PileOnlyFlgMask = 0x00004000;
static const uint32_t PileOnlyFlgShft = 14;
static const uint32_t PileupFlgMask = 0x00008000;
static const uint32_t PileupFlgShft = 15;
static const uint32_t LstDscTsLoMask = 0xffff0000;
static const uint32_t LstDscTsLoShft = 0;

//dword 4
static const uint32_t LstDscTsHiMask = 0xffff0000;
static const uint32_t LstDscTsHiShft = 0;
static const uint32_t LstDscTsHiUpShft = 16;
static const uint32_t CfdSmp0Mask = 0x3fff0000;
static const uint32_t CfdSmp0Shft = 16;

//dword 5
static const uint32_t SmpledBaseMask = 0x00ffffff;
static const uint32_t SmpledBaseShft = 0;

//dword 6
static const uint32_t CfdSmp1Mask = 0x00003fff;
static const uint32_t CfdSmp1Shft = 0;
static const uint32_t CfdSmp2Mask = 0x3fff0000;
static const uint32_t CfdSmp2Shft = 16;

//dword 7
static const uint32_t PreRiseEnMask = 0x00ffffff;
static const uint32_t PreRiseEnShft = 0;
static const uint32_t PostRiseEnLoMask = 0xff000000;
static const uint32_t PostRiseEnLoShft = 24;

//dword 8
static const uint32_t PostRiseEnHiMask = 0x0000ffff;
static const uint32_t PostRiseEnHiShft = 0;
static const uint32_t PostRiseEnHiUpShft = 8;
static const uint32_t PkTsLoMask = 0xffff0000;
static const uint32_t PkTsLoShft = 16;

//dword 9
static const uint32_t PkTsHiMask = 0x0000ffff;
static const uint32_t PkTsHiShft = 0;
static const uint32_t PkTsHiUpShft = 16;

//dword 10
static const uint32_t M3EndSampMask = 0x00003fff;
static const uint32_t M3EndSampShft = 0;
static const uint32_t M3BegSampMask = 0x3fff0000;
static const uint32_t M3BegSampShft = 16;

//dword 11
static const uint32_t M2BegSampMask = 0x00003fff;
static const uint32_t M2BegSampShft = 0;
static const uint32_t M2EndSampMask = 0x3fff0000;
static const uint32_t M2EndSampShft = 16;

//dword 12
static const uint32_t PeakSampMask = 0x00003fff;
static const uint32_t PeakSampShft = 0;
static const uint32_t BaseSampMask = 0x3fff0000;
static const uint32_t BaseSampShft = 16;

// trace dwords
static const uint32_t LoSampleMask = 0x00003FFF;
static const uint32_t LoSampleMShft = 0;
static const uint32_t HiSampleMask = 0x3FFF0000;
static const uint32_t HiSampleMShft = 16;

/* GT event */
/* __contains both raw and */
/* __processed quantities! */

struct AgnosticSinglesInfo
{
    uint16_t channel;
    uint16_t ringId;
    uint16_t deltaDiscEvTime;
    double eventTime;
    double uncorEn;
    double corrEn;
    double baseParam;
};

/**********************
 * The structures below are pulled almost verbatim from a previous code
 **********************/
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
    float    m{0.0F};
    float    fthetaL{0.0F};
    float    fphiL{0.0F};
    float    fthetaR{0.0F};
    float    fphiR{0.0F};
    int32_t  eL{0};
    int32_t  eR{0};
    int32_t  rf{0};
};

struct Gamma
{
    uint64_t t{0};
    double   e{0.0F};
    uint16_t id{0};
    bool     BGO{false};
    bool     GE{false};
    bool     Compton{false};
};

struct DgsBuff
{
    unsigned int* DGSEventBuf[20]{nullptr};
};

struct DgsBase
{
    float    base_sum[Params::DGS::NumGsGeDet + 1]{0.0F};
    float    n_base[Params::DGS::NumGsGeDet + 1]{0.0F};
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
    double   e[MaxDgsNum]{0.0F};
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
    float    e{0.0F};
    float    angle{0.0F};
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
    float    x{0.0F};
    float    y{0.0F};
    float    tgppac{0.0F};
    float    ppacde{0.0F};
    float    de1{0.0F};
    float    de2{0.0F};
    float    etot{0.0F};
    uint64_t ts{0};
    bool     FPTrue{false};
};

struct MbEvent
{
    uint16_t nMB{0};
    uint16_t id[MaxMbNum]{0};
    uint64_t ts[MaxMbNum]{0};
    float    etot[MaxMbNum]{0.0F};
    float    etail[MaxMbNum]{0.0F};
};


struct DssdEvent
{
    uint16_t nDSSD{0};
    uint64_t ts[MaxDssdNum]{0};
    float    e[MaxDssdNum]{0.0F};
    uint16_t id[MaxDssdNum]{0};
    uint16_t type[MaxDssdNum]{0};
};

bool getEvBuf(gzFile fp, std::string const& fileName, GebHeader& hdr, uint8_t*& evtBuff, uint64_t& bufferSize);
void getEv(uint8_t* buffer, DgsEventNew& evt, DgsTrace& trc, bool readTrace);

// prototype declaration to avoid extra include
class Calibrator;

bool extractDirtyCoinsFromEvt(DirtyCoincidence& dc, DgsEventNew const& evt, Calibrator & cal, AgnosticSinglesInfo& ad);
void extractCleanCoinsFromDirty(DirtyCoincidence& dc, CleanCoincidence& cc);

} // namespace Reader::DGS

#endif  // DGSSORT_SRC_READER_DGSSTRUCTS_H
