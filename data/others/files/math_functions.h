#pragma once

#include <cmath>

#ifdef WIN32
#using <mscorlib.dll>
using namespace System;

// fast implementation of logarithm
inline float safeLog(double val) 
{
  __int64 longValue;
  double truncX = val + 1e-30;  
  longValue = System::BitConverter::DoubleToInt64Bits(truncX);
  float x = (longValue >> 32);
  return (x - 1072632447.0f) / 1512775.0f;
}

#else

// fast implementation of logarithm
inline float safeLog(double val) 
{
  return std::log(val);
}

#endif

//inline double safeLog(double in_dX)
//{
//  double truncX = __max(in_dX, 1e-30);
//
//  return log(truncX);
//}
