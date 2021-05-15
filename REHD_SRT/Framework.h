#pragma once

// Enable Visual Styles
#pragma comment(                                                                                                       \
    linker,                                                                                                            \
    "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Gdiplus.lib")

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "resource.h"

#include <CommCtrl.h>
//#pragma warning(push)
//#pragma warning(disable : 4458)
#include <gdiplus.h>
//#pragma warning(pop)

#include <memory>
#include <string>

#include "Utils.h"

const wchar_t appName[] = L"Biohazard HD Remaster SRT";
const int fontReferenceDPI{72};
const int windowsReferenceDPI{96};
