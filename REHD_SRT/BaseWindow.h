#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

template <class DERIVED_TYPE> class BaseWindow
{
  public:
    static auto CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        DERIVED_TYPE *pThis{nullptr};

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT *pCreate{reinterpret_cast<CREATESTRUCT *>(lParam)};
            pThis = static_cast<DERIVED_TYPE *>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = reinterpret_cast<DERIVED_TYPE *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow()
    {
    }

    auto Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hwndParent = 0, HMENU hMenu = 0) -> BOOL
    {
        WNDCLASS wc{0};

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hwndParent, hMenu,
                                GetModuleHandle(nullptr), this);

        return (m_hwnd ? TRUE : FALSE);
    }

    auto Window() const -> HWND
    {
        return m_hwnd;
    }

  protected:
    virtual auto ClassName() const -> PCWSTR = 0;
    virtual auto HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT = 0;

    HWND m_hwnd{nullptr};
};
