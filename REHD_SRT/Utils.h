#pragma once

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif // !WIN32_LEAN_AND_MEAN

#include "Framework.h"

template <class T, class U, HWND(U::*m_hWnd)> T *InstanceFromWndProc(HWND hWnd, UINT uMsg, LPARAM lParam)
{
    // Get the pointer to the class instance
    T *pInstance;
    if (uMsg == WM_NCCREATE)
    {
        // The pointer has been passed via CreateWindowExW and now needs to be saved via SetWindowLongPtrW.
        LPCREATESTRUCTW pCreateStruct{reinterpret_cast<LPCREATESTRUCTW>(lParam)};
        pInstance = reinterpret_cast<T *>(pCreateStruct->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance));

        // We are handling the first useful window message, so this is the perfect place to also set the hWnd member
        // variable.
        pInstance->*m_hWnd = hWnd;
    }
    else
    {
        // Get the saved pointer via SetWindowLongPtrW above.
        pInstance = reinterpret_cast<T *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    return pInstance;
}

auto GetWindowDpi(HWND hWnd) -> unsigned short;
auto SafeLoadSystemLibrary(const std::wstring &libraryFile) -> HMODULE;
auto LoadStringAsWstr(HINSTANCE hInstance, UINT uID) -> std::wstring;
auto LoadGIFAsGdiplusBitmap(HINSTANCE hInstance, UINT uID) -> std::unique_ptr<Gdiplus::Bitmap>;
auto LoadPNGAsGdiplusBitmap(HINSTANCE hInstance, UINT uID) -> std::unique_ptr<Gdiplus::Bitmap>;
