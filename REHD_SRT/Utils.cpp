#include "Utils.h"

typedef HRESULT(WINAPI *PGetDpiForMonitor)(HMONITOR hmonitor, int dpiType, UINT *dpiX, UINT *dpiY);

auto GetWindowDpi(HWND hWnd) -> unsigned short
{
    // Try to get the DPI setting for the monitor where the given window is located.
    // This API is Win8.1+.
    HMODULE hShcore{SafeLoadSystemLibrary(L"shcore.dll")};
    if (hShcore)
    {
        PGetDpiForMonitor getDpiForMonitor{
            reinterpret_cast<PGetDpiForMonitor>(GetProcAddress(hShcore, "GetDpiForMonitor"))};
        if (getDpiForMonitor)
        {
            HMONITOR hMonitor{MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY)};
            UINT uiDpiX;
            UINT uiDpiY;
            HRESULT hr{getDpiForMonitor(hMonitor, 0, &uiDpiX, &uiDpiY)};
            if (SUCCEEDED(hr))
            {
                return static_cast<unsigned short>(uiDpiX);
            }
        }
    }

    // We couldn't get the window's DPI above, so get the DPI of the primary monitor using an API that is
    // available in all Windows versions.
    HDC hScreenDC{GetDC(0)};
    int iDpiX{GetDeviceCaps(hScreenDC, LOGPIXELSX)};
    ReleaseDC(0, hScreenDC);

    return static_cast<unsigned short>(iDpiX);
}

auto SafeLoadSystemLibrary(const std::wstring &libraryFile) -> HMODULE
{
    // LOAD_LIBRARY_SEARCH_SYSTEM32 is only supported if KB2533623 is installed on Vista/Win7, and starting from Win8.
    // This update also adds SetDefaultDllDirectories, so we can query that API to check for KB2533623.
    FARPROC setDefaultDllDirectories{GetProcAddress(GetModuleHandleW(L"kernel32"), "SetDefaultDllDirectories")};
    if (setDefaultDllDirectories)
    {
        return LoadLibraryExW(libraryFile.c_str(), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }

    // LOAD_LIBRARY_SEARCH_SYSTEM32 is not supported, so use the next best LOAD_WITH_ALTERED_SEARCH_PATH instead.
    std::wstring libraryFilePath{MAX_PATH, L'\0'};
    UINT cch{GetSystemDirectoryW(libraryFilePath.data(), libraryFilePath.size())};
    if (cch == 0 || cch >= libraryFilePath.size())
    {
        return nullptr;
    }

    libraryFilePath.resize(cch);
    libraryFilePath += L"\\" + libraryFile;

    return LoadLibraryExW(libraryFilePath.data(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
}

auto LoadStringAsWstr(HINSTANCE hInstance, UINT uID) -> std::wstring
{
    PCWSTR pws;
    int cch{LoadStringW(hInstance, uID, reinterpret_cast<LPWSTR>(&pws), 0)};
    return std::wstring(pws, cch);
}

// auto LoadGIFAsGdiplusBitmap(HINSTANCE hInstance, UINT uID) -> std::unique_ptr<Gdiplus::Bitmap>
auto LoadGIFAsGdiplusBitmap(HINSTANCE hInstance, UINT uID) -> std::unique_ptr<Gdiplus::Image>
{
    HRSRC findResource{FindResourceW(hInstance, MAKEINTRESOURCEW(uID), L"GIF")};
    if (findResource == nullptr)
    {
        return nullptr;
    }

    DWORD dwSize{SizeofResource(hInstance, findResource)};
    if (dwSize == 0)
    {
        return nullptr;
    }

    HGLOBAL loadResource{LoadResource(hInstance, findResource)};
    if (loadResource == nullptr)
    {
        return nullptr;
    }

    const void *resource{LockResource(loadResource)};
    if (!resource)
    {
        return nullptr;
    }

    std::unique_ptr<Gdiplus::Image> bitmap;
    HGLOBAL buffer{GlobalAlloc(GMEM_MOVEABLE, dwSize)};
    if (buffer)
    {
        void *pBuffer{GlobalLock(buffer)};
        if (pBuffer)
        {
            CopyMemory(pBuffer, resource, dwSize);

            IStream *stream;
            if (CreateStreamOnHGlobal(pBuffer, FALSE, &stream) == S_OK)
            {
                bitmap.reset(Gdiplus::Image::FromStream(stream));
                stream->Release();
            }
            GlobalUnlock(pBuffer);
        }
        GlobalFree(buffer);
    }
    return bitmap;
}

auto LoadPNGAsGdiplusBitmap(HINSTANCE hInstance, UINT uID) -> std::unique_ptr<Gdiplus::Bitmap>
{
    HRSRC findResource{FindResourceW(hInstance, MAKEINTRESOURCEW(uID), L"PNG")};
    if (findResource == nullptr)
    {
        return nullptr;
    }

    DWORD dwSize{SizeofResource(hInstance, findResource)};
    if (dwSize == 0)
    {
        return nullptr;
    }

    HGLOBAL loadResource{LoadResource(hInstance, findResource)};
    if (loadResource == nullptr)
    {
        return nullptr;
    }

    const void *resource{LockResource(loadResource)};
    if (!resource)
    {
        return nullptr;
    }

    std::unique_ptr<Gdiplus::Bitmap> bitmap;
    HGLOBAL buffer{GlobalAlloc(GMEM_MOVEABLE, dwSize)};
    if (buffer)
    {
        void *pBuffer{GlobalLock(buffer)};
        if (pBuffer)
        {
            CopyMemory(pBuffer, resource, dwSize);

            IStream *stream;
            if (CreateStreamOnHGlobal(pBuffer, FALSE, &stream) == S_OK)
            {
                bitmap.reset(Gdiplus::Bitmap::FromStream(stream));
                stream->Release();
            }
            GlobalUnlock(pBuffer);
        }
        GlobalFree(buffer);
    }
    return bitmap;
}
