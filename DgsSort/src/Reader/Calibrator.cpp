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
#include"Calibrator.h"
// includes for C system headers
// includes for C++ system headers
#include<cmath>
#include<fstream>
#include<iostream>
// includes from other libraries
// includes from DgsSort

namespace Reader::DGS
{

Calibrator::Calibrator(const std::string & calFileName, double recoilBeta)
{
    // read the calibration data
    std::ifstream calFile(calFileName.c_str());
    std::string line;
    std::getline(calFile, line, '\n');
//    std::cout << line << '\n';
    std::getline(calFile, line, '\n');
//    std::cout << line << '\n';
    unsigned detNum;
    double tempOff;
    double tempGain;
    double tempPoleZero;
    unsigned tempDisabled;
    for(unsigned i=0; i<Params::DGS::NumGsGeDet; ++i)
    {
        calFile >> detNum >> tempOff >> tempGain >> tempPoleZero >> tempDisabled;
        offsets[detNum - 1] = tempOff;
        gains[detNum - 1] = tempGain;
        poleZero[detNum - 1] = std::pow(tempPoleZero, Params::DGS::PoleZeroExp);
        invPoleZero[detNum - 1] = 1.0 - poleZero[detNum - 1];
        disabled[detNum - 1] = (tempDisabled != 0);
        poleZeroBase[detNum - 1] = 0.0;
//        std::cout << std::setw(6) << detNum << std::setw(15) << offsets[detNum - 1] << std::setw(15)
//                  << gains[detNum - 1] << std::setw(15) << tempPoleZero
//                  << std::setw(15) << tempDisabled << "\n" << std::flush;
    }
    calFile.close();
    // now calculate the doppler corrections
    double gamma = 1.0 / std::sqrt(1.0 - (recoilBeta * recoilBeta));
    for(unsigned i=0; i<Params::DGS::NumGsGeDet; ++i)
    {
        double angleRadians = ((Params::DGS::DetThetas[i] * M_PI) / 180.0);
        double cosTh = std::cos(angleRadians);
        dopplerCorrs[i] = (1.0 - (recoilBeta * cosTh)) * gamma;
    }
}

void Calibrator::getEnergies(uint16_t channel, uint16_t detType,  double deltaDiscEvTime,
                             int32_t preRiseEn, int32_t postRiseEn, CalInfo& res)
{
    if(detType == Params::DGS::GeTypeDet)
    {
        double sum1 = (res.firstSum = static_cast<double>(preRiseEn) / Params::DGS::MWidthVal);
        double sum2 = static_cast<double>(postRiseEn) / Params::DGS::MWidthVal;
        if(deltaDiscEvTime >= Params::DGS::DeltaDiscEvTimeThreshold)
        {
            res.poleZeroBase = (poleZeroBase[channel] = sum1);
        }
        double temp  = sum2 - (sum1 * poleZero[channel]);
        temp -= poleZeroBase[channel] * invPoleZero[channel];
        res.unCorrEnergy = ((temp * gains[channel]) - offsets[channel]) + dist(rng);
        res.energy = (res.unCorrEnergy * dopplerCorrs[channel]);
    }
    else if(detType == Params::DGS::BgoTypeDet)
    {
        res.energy = 0.0; // since a corrected energy is not calculated here
        res.unCorrEnergy = static_cast<double>(postRiseEn - preRiseEn) / Params::DGS::MWidthVal;
    }
}


} // namespace Reader::DGS

