#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

constexpr int DEFAULT_MAX_PERCENTAGE = 10;

class CPULimiter
{
  private:
    LARGE_INTEGER m_lastTotalSystemTime;
    LARGE_INTEGER m_lastThreadUsageTime;
    int m_ratio{DEFAULT_MAX_PERCENTAGE};

  public:
    CPULimiter();
    CPULimiter(int ratio);

    BOOL CalculateAndSleep();
    inline void SetRatio(int ratio)
    {
        m_ratio = ratio;
    }
};
