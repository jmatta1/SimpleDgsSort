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
#ifndef DGSSORT_SRC_READER_CALIBRATOR_H
#define DGSSORT_SRC_READER_CALIBRATOR_H

// includes for C system headers
// includes for C++ system headers
#include<string>
#include<cstdint>
#include<random> // for the adapter to make random ints into doubles in -0.5 to 0.5
// includes from other libraries
#include<pcg/include/pcg_random.hpp> // for the fast random number generator with very good randomness
// includes from DgsSort
#include"Parameters/DgsParameters.h"

namespace Reader::DGS
{


struct CalInfo
{
    double unCorrEnergy{0.0};
    double energy{0.0};
    double firstSum{0.0};
    double poleZeroBase{0.0};
};

/*!
* @brief class to read a calibration file and recoil beta and then be used to calibrate all raw
* detector energies
*/
class Calibrator
{
public:
    Calibrator(std::string const& calFileName, double recoilBeta);

    bool isEnabled(uint16_t channel){return !disabled[channel];}
    void getEnergies(uint16_t channel,  uint16_t detType,   double deltaDiscEvTime,
                     int32_t preRiseEn, int32_t postRiseEn, CalInfo& res);

private:
    std::uniform_real_distribution<double> dist{-0.5, 0.5};
    pcg_extras::seed_seq_from<std::random_device> seed_source;
    pcg64 rng{seed_source};

    double offsets[Params::DGS::NumGsGeDet];
    double gains[Params::DGS::NumGsGeDet];
    double poleZero[Params::DGS::NumGsGeDet];
    double invPoleZero[Params::DGS::NumGsGeDet];
    double dopplerCorrs[Params::DGS::NumGsGeDet];
    // honestly I don't quite get this thing yet some kind of baseline correction?
    double poleZeroBase[Params::DGS::NumGsGeDet];
    bool disabled[Params::DGS::NumGsGeDet];
};

} // namespace Reader::DGS

#endif  // DGSSORT_SRC_READER_CALIBRATOR_H
