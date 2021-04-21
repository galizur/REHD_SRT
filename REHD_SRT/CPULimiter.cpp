#include "CPULimiter.h"

CPULimiter::CPULimiter()
{
}

CPULimiter::CPULimiter(int ratio) : m_ratio{ratio}
{
}

BOOL CPULimiter::CalculateAndSleep()
{
    FILETIME sysidle, kerusage, userusage, threadkern, threaduser, threadcreat, threadexit;
    LARGE_INTEGER tmpvar, thissystime, thisthreadtime;

    if (!::GetSystemTimes(&sysidle, &kerusage, &userusage))
    {
        return FALSE;
    }

    if (!::GetThreadTimes(GetCurrentThread(), &threadcreat, &threadexit, &threadkern, &threaduser))
    {
        return FALSE;
    }

    tmpvar.LowPart = sysidle.dwLowDateTime;
    tmpvar.HighPart = sysidle.dwHighDateTime;
    thissystime.QuadPart = tmpvar.QuadPart;

    tmpvar.LowPart = kerusage.dwLowDateTime;
    tmpvar.HighPart = kerusage.dwHighDateTime;
    thissystime.QuadPart = thissystime.QuadPart + tmpvar.QuadPart;

    tmpvar.LowPart = userusage.dwLowDateTime;
    tmpvar.HighPart = userusage.dwHighDateTime;
    thissystime.QuadPart = thissystime.QuadPart + tmpvar.QuadPart;

    tmpvar.LowPart = threadkern.dwLowDateTime;
    tmpvar.HighPart = threadkern.dwHighDateTime;
    thisthreadtime.QuadPart = tmpvar.QuadPart;

    tmpvar.LowPart = threaduser.dwLowDateTime;
    tmpvar.HighPart = threaduser.dwHighDateTime;
    thisthreadtime.QuadPart = thisthreadtime.QuadPart + tmpvar.QuadPart;

    if (thisthreadtime.QuadPart != 0 && (((thisthreadtime.QuadPart - m_lastThreadUsageTime.QuadPart) * 100) -
                                         ((thissystime.QuadPart - m_lastTotalSystemTime.QuadPart) * m_ratio)) > 0)
    {
        // Calculate the time interval to sleep for averaging the extra CPU usage
        // by this thread.

        LARGE_INTEGER timetosleepin100ns;
        timetosleepin100ns.QuadPart = (((thisthreadtime.QuadPart - m_lastThreadUsageTime.QuadPart) * 100) / m_ratio) -
                                      (thissystime.QuadPart - m_lastTotalSystemTime.QuadPart);

        // check if time is less than a millisecond, if yes, keep it for next time.
        if ((timetosleepin100ns.QuadPart / 10000) <= 0)
            return FALSE;

        // Time to Sleep :)
        Sleep(timetosleepin100ns.QuadPart / 10000);
    }

    // Copy usage time values for next time calculations.
    m_lastTotalSystemTime.QuadPart = thissystime.QuadPart;
    m_lastThreadUsageTime.QuadPart = thisthreadtime.QuadPart;
    return TRUE;
}
