#include "UserInterface.h"

UserInterface::UserInterface()
{
    m_bhd = std::make_unique<Biohazard>();
}

auto UserInterface::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Get DPI setting of the monitor the window is shown
        m_currentDPI = GetWindowDPI(this->Window());
        // Create font
        wcscpy_s(m_lfguiFont.lfFaceName, L"Courier New");
        m_lfguiFont.lfHeight = -MulDiv(16, m_currentDPI, 72);
        m_guiFont = CreateFontIndirectW(&m_lfguiFont);
        // Set main window size
        int iHeight = MulDiv(130, m_currentDPI, 96);
        int iWidth = MulDiv(325, m_currentDPI, 96);
        SetWindowPos(this->Window(), nullptr, 0, 0, iWidth, iHeight, SWP_NOMOVE);

        ShowWindow(this->Window(), m_nShowCmd);

        CreateEditControls();

        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc{BeginPaint(m_hwnd, &ps)};
        SetColors(hdc, &ps);
        // HFONT hOldFont{CreateUserFont(hdc)};
        // colors and stuff if we want
        if (!m_readyToShow)
        {
            DrawWaitingText(hdc);
        }
        else
        {
            CreateStaticControls(hdc);
            // Set Edit controls to visible
            ShowWindow(m_hHealthPlayer, SW_SHOW);
            ShowWindow(m_hHealthEnemyOne, SW_SHOW);
            ShowWindow(m_hHealthEnemyTwo, SW_SHOW);
        }
        // SelectObject(hdc, hOldFont);
        // DeleteObject(m_guiFont);
        EndPaint(m_hwnd, &ps);
    }
        return 0;

    case WM_SYSCOLORCHANGE:
        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;

    case WM_SIZE:
        m_padding = MulDiv(5, m_currentDPI, 96);

        m_cyChar = -MulDiv(16, m_currentDPI, 72);
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

auto UserInterface::RetrieveValues() -> void
{
    do
    {
        m_bhd->FindProcessHandle();
        m_bhd->FindBaseAddress();

    } while (m_bhd->GetProcessHandle() == 0 || m_bhd->GetBaseAddress() == 0);

    if (m_bhd->GetProcessHandle() != 0 && m_bhd->GetBaseAddress() != 0)
    {
        m_readyToShow = true;
        InvalidateRect(m_hwnd, nullptr, TRUE);
    }
    while (true)
    {
        CalculateHealthValues();
        Sleep(300);
    }
}

auto UserInterface::GetWindowDPI(HWND hwnd) -> WORD
{
    typedef HRESULT(WINAPI * PGetDpiForMonitor)(HMONITOR hmonitor, int dpiType, UINT *dpiX, UINT *dpiY);

    HMODULE hShcore{LoadLibraryW(L"shcore")};
    if (hShcore)
    {
        PGetDpiForMonitor pGetDpiForMonitor =
            reinterpret_cast<PGetDpiForMonitor>(GetProcAddress(hShcore, "GetDpiForMonitor"));
        if (pGetDpiForMonitor)
        {
            HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
            UINT uiDpiX;
            UINT uiDpiY;
            HRESULT hr{pGetDpiForMonitor(hMonitor, 0, &uiDpiX, &uiDpiY)};
            if (SUCCEEDED(hr))
            {
                return static_cast<WORD>(uiDpiX);
            }
        }
    }

    HDC hScreenDC{GetDC(0)};
    int iDpiX{GetDeviceCaps(hScreenDC, LOGPIXELSX)};
    ReleaseDC(0, hScreenDC);

    return static_cast<WORD>(iDpiX);
}

auto UserInterface::SetColors(HDC hdc, PAINTSTRUCT *ps) -> void
{
    SetGraphicsMode(hdc, GM_ADVANCED);
    FillRect(hdc, &ps->rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
    SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
}

auto UserInterface::CreateStaticControls(HDC hdc) -> void
{
    ExtTextOut(hdc, 0 + m_padding, 0 + m_padding, 0, nullptr, LoadStringAsWstr(IDS_STRING102).c_str(),
               wcslen(LoadStringAsWstr(IDS_STRING102).c_str()), nullptr);
    ExtTextOut(hdc, 0 + m_padding, m_cyChar + 20, 0, nullptr, LoadStringAsWstr(IDS_STRING103).c_str(),
               wcslen(LoadStringAsWstr(IDS_STRING103).c_str()), nullptr);
    ExtTextOut(hdc, 0 + m_padding, m_cyChar + 50, 0, nullptr, LoadStringAsWstr(IDS_STRING104).c_str(),
               wcslen(LoadStringAsWstr(IDS_STRING104).c_str()), nullptr);
}

auto UserInterface::CreateEditControls() -> void
{
    m_hHealthPlayer = CreateWindowEx(WS_EX_STATICEDGE, L"Edit", 0, WS_CHILD | SS_LEFT | WS_DISABLED, 200, 0 + m_padding,
                                     100, 20, m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
    m_hHealthEnemyOne =
        CreateWindowEx(WS_EX_STATICEDGE, L"Edit", 0, WS_CHILD | SS_LEFT | WS_DISABLED, 200, 30 + m_padding, 100, 20,
                       m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
    m_hHealthEnemyTwo =
        CreateWindowEx(WS_EX_STATICEDGE, L"Edit", 0, WS_CHILD | SS_LEFT | WS_DISABLED, 200, 60 + m_padding, 100, 20,
                       m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
}

auto UserInterface::CreateUserFont(HDC hdc) -> HFONT
{
    m_guiFont = CreateFont(16, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Courier New");
    HFONT hOldFont = (HFONT)SelectObject(hdc, m_guiFont);
    GetTextMetrics(hdc, &m_tm);
    m_cxChar = m_tm.tmAveCharWidth;
    m_cyChar = m_tm.tmHeight + m_tm.tmExternalLeading;
    return hOldFont;
}

auto UserInterface::LoadStringAsWstr(UINT uID) -> std::wstring
{
    PCWSTR pws;
    int cch = LoadStringW(GetModuleHandleW(nullptr), uID, reinterpret_cast<LPWSTR>(&pws), 0);
    return std::wstring(pws, cch);
}

auto UserInterface::LoadStringAsWstr(HINSTANCE hInstance, UINT uID) -> std::wstring
{
    PCWSTR pws;
    int cch = LoadStringW(hInstance, uID, reinterpret_cast<LPWSTR>(&pws), 0);
    return std::wstring(pws, cch);
}

auto UserInterface::DrawWaitingText(HDC hdc) -> void
{
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    DrawTextW(hdc, LoadStringAsWstr(IDS_STRING101).c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    SetTextAlign(hdc, TA_CENTER | TA_TOP);
}

auto UserInterface::CalculateHealthValues() -> void
{
    int health{m_bhd->CalculatePlayersHealth()};
    if (m_pHealth != health)
    {
        m_pHealth = health;
        SetWindowText(m_hHealthPlayer, std::to_wstring(m_pHealth).c_str());
        InvalidateRect(m_hHealthPlayer, nullptr, TRUE);
        UpdateWindow(m_hHealthPlayer);
    }

    std::vector<int> eHealth{m_bhd->CalculateEnemiesHealth()};
    if (m_eHealth != eHealth)
    {
        m_eHealth = eHealth;
        SetWindowText(m_hHealthEnemyOne, std::to_wstring(m_eHealth[0]).c_str());
        InvalidateRect(m_hHealthEnemyOne, nullptr, TRUE);
        UpdateWindow(m_hHealthEnemyOne);
        SetWindowText(m_hHealthEnemyTwo, std::to_wstring(m_eHealth[1]).c_str());
        InvalidateRect(m_hHealthEnemyTwo, nullptr, TRUE);
        UpdateWindow(m_hHealthEnemyTwo);
    }
}
