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
#ifndef DGSSORT_SRC_PARAMETERS_DGSPARAMETERS_H
#define DGSSORT_SRC_PARAMETERS_DGSPARAMETERS_H

// includes for C system headers
// includes for C++ system headers
#include<cstdint>
// includes from other libraries
// includes from DgsSort

namespace Params::DGS
{

static const uint32_t NumGsGeDet = 110;
static const uint32_t NumModules = 30;
static const uint32_t NumRings = 17;

static const uint16_t GsMap[500]{  2,   4,   6,   8,  10,   2,   4,   6,   8,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  12,  40,  16,  18,  20,  12,  40,  16,  18,  20,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  22,  24,  26,  28,  30,  22,  24,  26,  28,  30,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  32,  34,  36,  38,  14,  32,  34,  36,  38,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  42,  44,  46,  48,  50,  42,  44,  46,  48,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  52,  54,  56,  58,  60,  52,  54,  56,  58,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  62,  64,  78,  68,  70,  62,  64,  78,  68,  70,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  72,  74,  76,  66,  80,  72,  74,  76,  66,  80,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  82,  84,  86,  88,  90,  82,  84,  86,  88,  90,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  92,  94,  96,  98, 100,  92,  94,  96,  98, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                 102, 104, 106, 108, 110, 102, 104, 106, 108, 110,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                   1,   3,   5,   7,   9,   1,   3,   5,   7,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  11,  13,  15,  17,  19,  11,  13,  15,  17,  19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  21,  23,  25,  27,  29,  21,  23,  25,  27,  29,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  31,  33,  35,  37,  39,  31,  33,  35,  37,  39,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  41,  43,  45,  47,  49,  41,  43,  45,  47,  49,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  51,  53,  55,  57,  59,  51,  53,  55,  57,  59,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  61,  63,  65,  67,  69,  61,  63,  65,  67,  69,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  71,  73,  75,  77,  79,  71,  73,  75,  77,  79,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  81,  97,  85,  87,  89,  81,  97,  85,  87,  89,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                  91,  93,  95,  83,  99,  91,  93,  95,  83,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                 101, 103, 105, 107, 109, 101, 103, 105, 107, 109,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

static const uint16_t ModMap[10]{1, 1, 1, 1, 1, 2, 2, 2, 2, 2};

static const double RingThetas[17]{  17.27465,  31.71747,  37.37737,  50.06504,  58.28253,  69.82033,  79.18768,   80.7096,      90.0,   99.2904,
                                    100.81232, 110.17967, 121.71747, 129.93496, 142.62263, 148.28253, 162.72535};

static const double DetThetas[110]{  17.27465,  17.27465,  17.27465,  17.27465,  31.71747,  17.27465,  31.71747,  31.71747,  31.71747,  31.71747,
                                     37.37737,  37.37737,  37.37737,  37.37737,  50.06504,  37.37737,  50.06504,  50.06504,  50.06504,  50.06504,
                                     50.06504,  50.06504,  50.06504,  50.06504,  58.28253,  50.06504,  58.28253,  58.28253,  69.82033,  58.28253,
                                     69.82033,  58.28253,  69.82033,  69.82033,  69.82033,  69.82033,  69.82033,  69.82033,  79.18768,  69.82033,
                                     79.18768,  69.82033,   80.7096,  79.18768,   80.7096,  79.18768,   80.7096,  79.18768,        90,   80.7096,
                                           90,   80.7096,        90,        90,        90,        90,        90,        90,   99.2904,        90,
                                      99.2904,        90, 100.81232,   99.2904, 100.81232,   99.2904, 100.81232,   99.2904, 110.17967, 100.81232,
                                    110.17967, 100.81232, 110.17967, 110.17967, 110.17967, 110.17967, 110.17967, 110.17967, 121.71747, 110.17967,
                                    121.71747, 110.17967, 121.71747, 121.71747, 129.93496, 121.71747, 129.93496, 129.93496, 129.93496, 129.93496,
                                    129.93496, 129.93496, 129.93496, 129.93496, 142.62263, 129.93496, 142.62263, 142.62263, 142.62263, 142.62263,
                                    148.28253, 148.28253, 148.28253, 148.28253, 162.72535, 148.28253, 162.72535, 162.72535, 162.72535, 162.72535};

static const double DetPhis[110  ]{    72,    216,    144,    288,     36,    360,    108,    252,    180,    324,
                                     72,    216,    144,    288,  22.84,    360,  49.16, 193.16,  94.84, 238.84,
                                 121.16, 265.16, 166.84, 310.84,     72, 337.16,    144,    216,  18.49,    288,
                                  53.51,    360,  90.49, 197.51, 125.51, 234.49, 162.49, 269.51,     72, 306.49,
                                    144, 341.51,     36,    216,    108,    288,    180,    360,     18,    252,
                                     54,    324,     90,    198,    126,    234,    162,    270,     72,    306,
                                    144,    342,     36,    216,    108,    288,    180,    360,  17.51,    252,
                                  54.49,    324,  89.51, 198.49, 126.49, 233.51, 161.51, 270.49,     36, 305.51,
                                    108, 342.49,    180,    252,  13.16,    324,  58.84, 202.84,  85.16, 229.16,
                                 130.84, 274.84, 157.16, 301.16,     36, 346.84,    108,    252,    180,    324,
                                     72,    216,    144,    288,     36,    360,    108,    252,    180,    324};


} // namespace Params::DGS

#endif  // DGSSORT_SRC_PARAMETERS_DGSPARAMETERS_H
