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

#include"DgsReader.h"
// includes for C system headers
// includes for C++ system headers
#include<iostream>
#include<string>
#include<cstdlib>
#include<cstdint>
#include<cstring>
#include<new>
// includes from other libraries
#include<zlib.h>
// includes from DgsSort
#include"Utility/EndianHandling.h"

namespace Reader::DGS
{

//// wtf, static modifying a function that is not a member function... it basically does nothing...
//// oh I know why because you both include the file and add it to the command line
//// DON'T DO THAT!!!!!!
//static unsigned int* GetEvBuf(gzFile fp, std::string const& fileName, GebHeader* hdr)
//{
//    unsigned int* EventBuf;
//    //if(fread(Header,GEBHDRLENBYTES,1,fp) != 1){
//    // reading _straight_ into structs... ffs you _DO NOT DO THIS_
//    // there is no guarantee that things stay the same between _compiler versions_ let alone architectures
//    // I suppose at least it's a _little_ struct...
//    // ... ok no no no no no, I am going to fricking _fix_ this
//    //    if(gzread(fp, hdr, GEBHDRLENBYTES) != GEBHDRLENBYTES)
//    {
//        //if(feof(fp)){
//        if(gzeof(fp))
//        {
//            std::cout << "End of file " << fileName << std::endl;
//            return NULL;
//        }
//        std::cout << "File read error " << fileName << std::endl;
//        return NULL;
//    }
//    //OMG MALLOC EVERY FRICKING TIME??? NO WONDER TORBEN'S CODE IS SLOW _REUSE SHIT MAN!!!_
//    // /insert meme graphic: "I have contained my rage for as long as possible"
//    if(!(EventBuf = (unsigned int*) malloc(hdr->length)))
//    {
//        std::cout << "\007  ERROR: Could not malloc data buffer " << hdr->length << "%i bytes." << std::endl;;
//        exit(-1);
//    }
//    //fread(EventBuf,hdr->length,1,fp);
//    gzread(fp, EventBuf, hdr->length);
//    return EventBuf;
//}

static const uint64_t CacheLineAlignment = 64;
static const uint64_t CacheLineAlignmentMask = 63;

uint8_t* cacheAlignedAlloc(uint64_t& size)
{
    // ensure the size is a multiple of 64 or make it so
    // for powers of 2 bit mask is the same as modulus and much faster
    if((size & CacheLineAlignmentMask) != 0)
    {
        size += CacheLineAlignment - (size & CacheLineAlignmentMask);
    }
    return static_cast<uint8_t*>(std::aligned_alloc(CacheLineAlignment, size));
}

// the event buffer passed in must always be allocated by CacheAlignedAlloc
bool getEvBuf(gzFile fp, std::string const& fileName, GebHeader* hdr, uint8_t*& evtBuff, uint64_t& bufferSize)
{
    char headerBuffer[GebHdrLenBytes] = {0};

    if(gzread(fp, headerBuffer, GebHdrLenBytes) != GebHdrLenBytes)
    {
        if(gzeof(fp))
        {
            std::cout << "End of file " << fileName << "\n";
            return false;
        }
        std::cout << "File read error " << fileName << "\n";
        return false;
    }

    // now transfer things from the header buffer into the header
    // I know it looks cumbersom, but the compiler will mash this down to something very fast and clean
    size_t offset = 0;
    std::memcpy(&hdr->type,   headerBuffer + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    std::memcpy(&hdr->length, headerBuffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);
    std::memcpy(&hdr->type,   headerBuffer + offset, sizeof(uint64_t));

    // now see if we need to (re)allocate the event buffer
    if(hdr->length > bufferSize)
    {
        std::free(evtBuff);
        bufferSize = hdr->length;
        evtBuff = cacheAlignedAlloc(bufferSize);
        if(evtBuff == nullptr)
        {
            std::cout << "\007  ERROR: Could not malloc data buffer " << hdr->length << "%i bytes.\n" << std::flush;
            throw std::bad_alloc{};
        }
    }

    //fread(EventBuf,hdr->length,1,fp);
    gzread(fp, static_cast<void*>(evtBuff), hdr->length);
    //for safeties sake, zero the end of the buffer just in case
    return true;
}

// sigh.. another torben special, let's fix this too so I don't shudder when it is called
//int GetEv(unsigned int* TEMP, DGSEVENTNEW* DGS, DGSTRACE* TRACE)
//{
//    int  i;
//    //int  WF;
//    //unsigned short int  headertype;

// torben is using htobe32 here...
// but he should logically be using be32toh since the value is starting in Big Endian format
// and is then being converted into host format (little endian on just about everything these days)
// however, the operation an involution (is its own inverse) so the two function give the same result
// on the other hand HE IS CALLING IT MULTIPLE TIMES PER CELL IN THE ARRAY, WTF!!!!!!!!!

//    //headertype                  = (unsigned short int) ((htobe32(TEMP[2]) & 0x000f0000) >> 16);
//    //printf("headertype = %i\n",headertype);

//    DGS->chan_id                = (unsigned short int)(htobe32(TEMP[0]) & 0xf);
//    DGS->board_id               = (unsigned short int)((htobe32(TEMP[0]) & 0xfff0) >> 4);
//    DGS->event_length           = (unsigned short int)((htobe32(TEMP[0]) & 0x7ff0000) >> 16);
//    DGS->geo_addr               = (unsigned short int)((htobe32(TEMP[0]) & 0xf8000000) >> 27);
//    DGS->event_timestamp        = (unsigned long long int)(htobe32(TEMP[1]) & 0xffffffff);
//    DGS->event_timestamp       += (((unsigned long long int)(htobe32(TEMP[2]) & 0x0000ffff))  << 32);

// Torben calls the cells of the array words though they are 4 bytes
// according to Intel, AMD, ARM, _and_ all the big compiler makers, a word is 2-bytes, 4-byte blocks
// are double words, aka dwords. an 8-byte block is a quad word (aka qword)

//    //DGS->write_flag             = (unsigned short int) ((htobe32(TEMP[3]) & 0x00000100) >> 5);          // Word 3: 5
//    DGS->timestamp_match_flag   = (unsigned short int)((htobe32(TEMP[3]) & 0x00000080) >> 7);           // Word 3: 7
//    DGS->external_disc_flag     = (unsigned short int)((htobe32(TEMP[3]) & 0x00000100) >> 8);           // Word 3: 8
//    DGS->peak_valid_flag        = (unsigned short int)((htobe32(TEMP[3]) & 0x00000200) >> 9);           // Word 3: 9
//    DGS->offset_flag            = (unsigned short int)((htobe32(TEMP[3]) & 0x00000400) >> 10);          // Word 3: 10
//    DGS->cfd_valid_flag         = (unsigned short int)((htobe32(TEMP[3]) & 0x00000800) >> 11);          // Word 3: 11
//    DGS->sync_error_flag        = (unsigned short int)((htobe32(TEMP[3]) & 0x00001000) >> 12);          // Word 3: 12
//    DGS->general_error_flag     = (unsigned short int)((htobe32(TEMP[3]) & 0x00002000) >> 13);          // Word 3: 13
//    DGS->pileup_only_flag       = (unsigned short int)((htobe32(TEMP[3]) & 0x00004000) >> 14);          // Word 3: 14
//    DGS->pileup_flag            = (unsigned short int)((htobe32(TEMP[3]) & 0x00008000) >> 15);          // Word 3: 15
//    //DGS->last_disc_timestamp    = (((unsigned long long int)(htobe32(TEMP[3]) & 0xffff0000)) >> 16 );   // Word 3: 31..16 & LED MODE
//    //DGS->last_disc_timestamp   += (((unsigned long long int)(htobe32(TEMP[4]) & 0xffffffff)) << 16);    // Word 4 :31..0 & LED MODE
//    DGS->last_disc_timestamp    = (((unsigned long long int)(htobe32(TEMP[3]) & 0xffff0000)) >> 16);    // Word 3: 31..16 & CFD MODE
//    DGS->last_disc_timestamp   += (((unsigned long long int)(htobe32(TEMP[4]) & 0x00003fff)) << 16);    // Word 4 :31..0 & CFD MODE
//    DGS->cfd_sample_0           = (short int)((htobe32(TEMP[4]) & 0x3fff0000) >> 16);                   // Word 4: 29..16
//    DGS->sampled_baseline       = ((htobe32(TEMP[5]) & 0x00ffffff) >> 0);                               // Word 5: 23..0
//    DGS->cfd_sample_1           = (short int)((htobe32(TEMP[6]) & 0x00003fff) >> 0);                    // Word 6: 13..0
//    DGS->cfd_sample_2           = (short int)((htobe32(TEMP[6]) & 0x3fff0000) >> 16);                   // Word 6: 29..16
//    DGS->pre_rise_energy        = ((htobe32(TEMP[7]) & 0x00ffffff) >> 0);                               // Word 7: 23..0
//    DGS->post_rise_energy       = ((htobe32(TEMP[7]) & 0xff000000) >> 24);
//    DGS->post_rise_energy      += ((htobe32(TEMP[8]) & 0x0000ffff) << 8);
//    DGS->peak_timestamp         = (((unsigned long long int)(htobe32(TEMP[8]) & 0xffff0000)) >> 16);    // Word 8: 31..16 &
//    DGS->peak_timestamp        += (((unsigned long long int)(htobe32(TEMP[9]) & 0x0000ffff)) << 16);    // Word 8: 31..16 &
//    DGS->m3_end_sample          = (unsigned short int)((htobe32(TEMP[10]) & 0x00003fff) >> 0);          // Word 11:13..0
//    DGS->m3_begin_sample        = (unsigned short int)((htobe32(TEMP[10]) & 0x3fff0000) >> 16);         // Word 10:29..16
//    DGS->m2_begin_sample        = (unsigned short int)((htobe32(TEMP[11]) & 0x00003fff) >> 0);          // Word 11:13..0
//    DGS->m2_end_sample          = (unsigned short int)((htobe32(TEMP[11]) & 0x3fff0000) >> 16);         // Word 11:29..16
//    DGS->peak_sample            = (unsigned short int)((htobe32(TEMP[12]) & 0x00003fff) >> 0);          // Word 12:13..0
//    DGS->base_sample            = (unsigned short int)((htobe32(TEMP[12]) & 0x3fff0000) >> 16);         // Word 12:29..16

//    //WF = (int) ((htobe32(TEMP[3]) & 0x00000020) >> 5);          // Word 3: 5
//    //printf("WF=%i\n",WF);
//    TRACE->Len = 0;
//    for(i = HDRLENINTS; i < DGS->event_length; i++)
//    {
//        if(i < 1037)
//        {
//            //if(i==HDRLENINTS){
//            //  printf("%x,%x; ",htobe32(TEMP[i]) & 0xffff,htobe32(TEMP[i]) & 0x3fff);
//            //  printf("%x,%x\n",(htobe32(TEMP[i]) >> 16) & 0xffff,(htobe32(TEMP[i]) >> 16) & 0x3fff);
//            //}
//            //if(WF==0){
//            TRACE->trace[2 * (i - HDRLENINTS)] = (short int)(htobe32(TEMP[i]) & 0x3fff);
//            TRACE->trace[2 * (i - HDRLENINTS) + 1] = (short int)((htobe32(TEMP[i]) >> 16) & 0x3fff);
//            TRACE->Len += 2;
//            //}
//        }
//    }
//    for(i = TRACE->Len; i < 1024; i++)
//    {
//        TRACE->trace[i] = 0;
//    }
//    return 1;
//}




static size_t readAndSwapFromAddress(uint8_t* addr, uint32_t& val)
{
    memcpy(&val, addr, sizeof(val));
    val = be32toh(val);
    return sizeof(uint32_t);
}

template<typename RetType>
RetType msc(uint32_t val, uint32_t mask, uint32_t shift) // mask, shift, and cast
{

    return static_cast<RetType>(((val & mask) >> shift));
}

int getEv(uint8_t* buffer, DgsEventNew* evt, DgsTrace* trc)
{
    uint32_t val{0};

    // dword 0
    uint64_t offset = readAndSwapFromAddress(buffer, val);
    evt->chan_id      = msc<uint16_t>(val, ChanIdMask,   ChanIdShft);
    evt->board_id     = msc<uint16_t>(val, BoardIdMask,  BoardIdShft);
    evt->event_length = msc<uint16_t>(val, EventLenMask, EventLenShft);
    evt->geo_addr     = msc<uint16_t>(val, GeoAddrMask,  GeoAddrShft);

    // dword 1
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->event_timestamp  = msc<uint64_t>(val, EvtTsLoMask, EvtTsLoShft);

    // dword 2
    offset += readAndSwapFromAddress(buffer + offset, val);
    // when adding, if the set bits in one value are zero in the other (like here)
    // then, in that case, |= (bit-wise or and assign) is equivalent to += (addition and assign)
    // AND it is (slightly) faster
    evt->event_timestamp |= (msc<uint64_t>(val, EvtTsHiMask, EvtTsHiShft) << EvtTsHiUpShft);

    //dword 3
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->timestamp_match_flag = msc<uint16_t>(val, TsMatchFlgMask,  TsMatchFlgShft);
    evt->external_disc_flag   = msc<uint16_t>(val, ExtDiscFlgMask,  ExtDiscFlgShft);
    evt->peak_valid_flag      = msc<uint16_t>(val, PkValidFlgMask,  PkValidFlgShft);
    evt->offset_flag          = msc<uint16_t>(val, OffsetFlgMask,   OffsetFlgShft);
    evt->cfd_valid_flag       = msc<uint16_t>(val, CfdValidFlgMask, CfdValidFlgShft);
    evt->sync_error_flag      = msc<uint16_t>(val, SyncErrFlgMask,  SyncErrFlgShft);
    evt->general_error_flag   = msc<uint16_t>(val, GenErrFlgMask,   GenErrFlgShft);
    evt->pileup_only_flag     = msc<uint16_t>(val, PileOnlyFlgMask, PileOnlyFlgShft);
    evt->pileup_flag          = msc<uint16_t>(val, PileupFlgMask,   PileupFlgShft);
    evt->last_disc_timestamp  = msc<uint64_t>(val, LstDscTsLoMask,  LstDscTsLoShft);

    //dword 4
    offset += readAndSwapFromAddress(buffer + offset, val);
    // using bitwise or instead of add here too, since we can get away with it
    evt->last_disc_timestamp |= (msc<uint64_t>(val, LstDscTsHiMask,  LstDscTsHiShft) << LstDscTsHiUpShft);
    evt->cfd_sample_0         =  msc< int16_t>(val, CfdSmp0Mask,     CfdSmp0Shft);

    //dword 5
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->sampled_baseline = msc< int32_t>(val, SmpledBaseMask, SmpledBaseShft);

    //dword 6
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->cfd_sample_1 = msc< int16_t>(val, CfdSmp1Mask, CfdSmp1Shft);
    evt->cfd_sample_2 = msc< int16_t>(val, CfdSmp1Mask, CfdSmp1Shft);

    //dword 7
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->pre_rise_energy  = msc< int32_t>(val, PreRiseEnMask,    PreRiseEnShft);
    evt->post_rise_energy = msc< int32_t>(val, PostRiseEnLoMask, PostRiseEnLoShft);

    //dword 8
    offset += readAndSwapFromAddress(buffer + offset, val);
    // again, bitwise or works instead of addition here
    evt->post_rise_energy |= (msc< int32_t>(val, PostRiseEnHiMask, PostRiseEnHiShft) << PostRiseEnHiUpShft);// NOLINT A bitwise operation with a signed type is safe here
    evt->peak_timestamp    =  msc<uint64_t>(val, PkTsLoMask, PkTsLoShft);

    //dword 9
    offset += readAndSwapFromAddress(buffer + offset, val);
    // bitwise or works instead of addition here
    evt->peak_timestamp   |= (msc<uint64_t>(val, PkTsHiMask, PkTsHiShft) << PkTsHiUpShft);

    //dword 10
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->m3_end_sample   = msc<uint16_t>(val, M3EndSampMask, M3EndSampShft);
    evt->m3_begin_sample = msc<uint16_t>(val, M3BegSampMask, M3BegSampShft);


    //dword 11
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->m2_begin_sample = msc<uint16_t>(val, M2BegSampMask, M2BegSampShft);
    evt->m2_end_sample   = msc<uint16_t>(val, M2EndSampMask, M2EndSampShft);

    //dword 12
    offset += readAndSwapFromAddress(buffer + offset, val);
    evt->peak_sample = msc<uint16_t>(val, PeakSampMask, PeakSampShft);
    evt->base_sample = msc<uint16_t>(val, BaseSampMask, BaseSampShft);

    // trace dwords
    trc->Len = 0;
    const uint32_t bound1 = (sizeof(uint32_t) * evt->event_length);
    const uint32_t bound2 = DgsTraceMaxLen - 1;
    while((offset < bound1) && (trc->Len < bound2))
    {
        offset += readAndSwapFromAddress(buffer + offset, val);
        trc->trace[trc->Len]     = msc<int16_t>(val, LoSampleMask, LoSampleMShft);
        trc->trace[trc->Len + 1] = msc<int16_t>(val, HiSampleMask, HiSampleMShft);
        trc->Len += 2;
    }

    // zero out additional samples torben does this with a loop... why?
    // memset is your _friend_
    if(trc->Len < DgsTraceMaxLen)
    {
        std::memset(trc->trace + trc->Len, 0, sizeof(int16_t)*(DgsTraceMaxLen - trc->Len));
    }
    return 1;//why return at all? why not void??
}


} // namespace Reader::DGS
