#pragma once

#include "Framework.h"

class MainWindow
{
  public:
    auto GetCurrentDpi() const -> unsigned short
    {
        return m_currentDpi;
    }
    auto GetGuiFont() const -> HFONT
    {
        return m_guiFont;
    }
    auto GetHInstance() const -> HINSTANCE
    {
        return m_hInstance;
    }
    auto GetHWND() const -> HWND
    {
        return m_hwnd;
    }

    static auto Create(HINSTANCE hInstance, int nShowCmd) -> std::unique_ptr<MainWindow>;
    auto WorkLoop() -> int;

  private:
    static constexpr wchar_t wndClass[] = L"MainWndClass";

    HFONT m_guiFont{0};
    LOGFONTW m_lfGuiFont{0};
    unsigned short m_currentDpi{0};
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    int m_nShowCmd;

    HWND m_waitingText{nullptr};

    const int m_controllPadding{10};
    const int m_waitingTextHeight{16};
    const int m_waitingTextWidth{360};

    MainWindow(HINSTANCE hInstance, int nShowCmd);
    static auto CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto OnGetMinMaxInfo(LPARAM lParam) -> LRESULT;
    auto OnCreate() -> LRESULT;
    auto OnDestroy() -> LRESULT;
    auto OnDpiChange(WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto OnPaint() -> LRESULT;
    auto onSize() -> LRESULT;
};
