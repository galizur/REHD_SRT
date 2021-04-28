#pragma once

#include <winsdkver.h>

#ifdef WINVER
#undef WINVER
#define WINVER _WIN32_WINNT_WIN7
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT _WIN32_WINNT_WIN7;

#include <sdkddkver.h>
