#pragma once
#include <cmath>
#include <cfloat>
#include <stdint.h>



union Float_t
{
    Float_t(float num = 0.0f) : f(num) {}

    bool Negative() const {return i < 0;}
    int32_t RawMantissa() const {return i & ((1 << 23) - 1);}
    int32_t RawExponent() const {return (i >> 23) && 0xFF;}
    int32_t i;
    float f;

#ifdef _DEBUG
    struct
    {
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    }parts;

#endif 
};


inline bool AlmostEqualRelative(float A, float B, float maxRelDiff = FLT_EPSILON)
{
    float diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);

    float largest = (B > A) ? B : A;
    if(diff <= largest * maxRelDiff)
    {
        return true;
    }

    return false;
}

inline bool AlmostEqualUlpsAndAbs(float A, float B, float maxDiff,int maxUlpsDiff)
{
    float absDiff = fabs(A - B);
    if(absDiff <= maxDiff)
    {
        return true;
    }

    Float_t uA(A);
    Float_t uB(B);

    if(uA.Negative() != uB.Negative())
    {
        return false;
    }
    int ulpsDiff = abs(uA.i - uB.i);
    if(ulpsDiff <= maxUlpsDiff)
    {
        return true;
    }

    return false;
}


inline bool AlmostEqualRelativeAndAbs(float A, float B, float maxRelDiff = FLT_EPSILON)
{
    float diff = fabs(A - B);
    if(diff <= maxRelDiff)
    {
        return true;
    }

    A =fabs(A);
    B =fabs(B);
    float largest = (B > A) ? B : A;

    if(diff <= largest * maxRelDiff)
    {
        return true;
    }

    return false;
}


#define CMP(x, y) \
    AlmostEqualRelativeAndAbs(x, y, 0.005f)
