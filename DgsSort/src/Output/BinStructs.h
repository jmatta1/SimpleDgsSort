/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 25 Sep, 2023
**
** @copyright Copyright (C) 2023 James Till Matta
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_OUTPUT_BINSTRUCTS_H
#define DGSSORT_SRC_OUTPUT_BINSTRUCTS_H

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from DgsSort

namespace Output
{

struct AxisParams
{
    int nb{0};
    double lo{0.0};
    double hi{0.0};
};

struct BinCalc2D
{
    double xlo{0.0};
    double xwd{0.0};
    double xhi{0.0};
    double ylo{0.0};
    double ywd{0.0};
    double yhi{0.0};
    int nbx{0};
    int bxh{0};
    int byh{0};

    constexpr BinCalc2D(AxisParams const& x, AxisParams const& y):
        xlo(x.lo), xwd((x.hi - x.lo)/x.nb), xhi(x.hi), ylo(y.lo), ywd((y.hi - y.lo)/y.nb),
        yhi(y.hi), nbx(x.nb + 2), bxh(x.nb + 1), byh(y.nb + 1) {}
    constexpr int operator()(double x, double y) const
    {
        int xb = ((x < xlo) ? 0 : ((x >= xhi) ? bxh : static_cast<int>((x-xlo)/xwd)));
        int yb = ((y < ylo) ? 0 : ((y >= yhi) ? byh : static_cast<int>((y-ylo)/ywd)));
        return (xb + (nbx * yb));
    }
};

// this variant will speed up the calc a little, just don't put in an invalid y
struct BinCalc2dIndexY
{
    double xlo{0.0};
    double xwd{0.0};
    double xhi{0.0};
    int nbx{0};
    int bxh{0};

    constexpr BinCalc2dIndexY(AxisParams const& x):
        xlo(x.lo), xwd((x.hi - x.lo)/x.nb), xhi(x.hi),
        nbx(x.nb + 2), bxh(x.nb + 1){}
    constexpr int operator()(double x, int y) const
    {
        int xb = ((x < xlo) ? 0 : ((x >= xhi) ? bxh : static_cast<int>((x-xlo)/xwd)));
        return (xb + (nbx * (y+1)));
    }
};

struct BinCalc3D
{
    double xlo{0.0};
    double xwd{0.0};
    double xhi{0.0};
    double ylo{0.0};
    double ywd{0.0};
    double yhi{0.0};
    double zlo{0.0};
    double zwd{0.0};
    double zhi{0.0};
    int nbx{0};
    int nby{0};
    int bxh{0};
    int byh{0};
    int bzh{0};

    constexpr BinCalc3D(AxisParams const& x, AxisParams const& y, AxisParams const& z):
        xlo(x.lo), xwd((x.hi - x.lo)/x.nb), xhi(x.hi), ylo(y.lo), ywd((y.hi - y.lo)/y.nb),
        yhi(y.hi), zlo(z.lo), zwd((z.hi - z.lo)/z.nb), zhi(z.hi), nbx(x.nb + 2),
        nby(x.nb + 2), bxh(x.nb + 1), byh(y.nb + 1), bzh(z.nb + 1) {}

    constexpr int operator()(double x, double y, double z) const
    {
        int xb = ((x < xlo) ? 0 : ((x >= xhi) ? bxh : static_cast<int>((x-xlo)/xwd) + 1));
        int yb = ((y < ylo) ? 0 : ((y >= yhi) ? byh : static_cast<int>((y-ylo)/ywd) + 1));
        int zb = ((z < zlo) ? 0 : ((z >= zhi) ? bzh : static_cast<int>((z-zlo)/zwd) + 1));
        return (xb + (nbx * (yb + (nby * zb))));
    }
};

// this variant will speed up the calc a little, just don't put in an invalid z
struct BinCalc3dIndexZ
{
    double xlo{0.0};
    double xwd{0.0};
    double xhi{0.0};
    double ylo{0.0};
    double ywd{0.0};
    double yhi{0.0};
    int nbx{0};
    int nby{0};
    int bxh{0};
    int byh{0};

    constexpr BinCalc3dIndexZ(AxisParams const& x, AxisParams const& y):
        xlo(x.lo), xwd((x.hi - x.lo)/x.nb), xhi(x.hi), ylo(y.lo), ywd((y.hi - y.lo)/y.nb),
        yhi(y.hi), nbx(x.nb + 2), nby(x.nb + 2), bxh(x.nb + 1), byh(y.nb + 1) {}
    constexpr int operator()(double x, double y, int z) const
    {
        int xb = ((x < xlo) ? 0 : ((x >= xhi) ? bxh : static_cast<int>((x-xlo)/xwd)));
        int yb = ((y < ylo) ? 0 : ((y >= yhi) ? byh : static_cast<int>((y-ylo)/ywd)));
        return (xb + (nbx * (yb + (nby * (z + 1)))));
    }
};


} // namespace Output

#endif  // DGSSORT_SRC_OUTPUT_BINSTRUCTS_H
