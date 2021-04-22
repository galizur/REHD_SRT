#pragma once

#include <sdkddkver.h>

#ifdef WINVER
#undef WINVER
#define WINVER _WIN32_WINNT_WIN7
#endif
