#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if !defined(WM_DPICHANGED)
#define WM_DPICHANGED 0x02E0
#endif

#include "TargetVer.h"

#include "resource.h"

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Constants.h"
