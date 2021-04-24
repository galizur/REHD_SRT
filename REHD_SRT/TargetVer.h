#pragma once

#include <sdkddkver.h>

#ifdef WINVER
#undef WINVER
#endif
#define WINVER _WIN32_WINNT_WIN7
