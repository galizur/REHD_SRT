#include "UserInterface.h"

auto UserInterface::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    static int cyChar;
    switch (uMsg)
    {
    case WM_CREATE:
        // Get text height
        cyChar = HIWORD(GetDialogBaseUnits());

        CreateEditControls();

        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        TEXTMETRIC tm;
        RECT rect;
        HDC hdc{BeginPaint(m_hwnd, &ps)};
        SetGraphicsMode(hdc, GM_ADVANCED);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
        SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
        HFONT hFont = CreateFont(16, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
                                 CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, L"Courier New");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        GetTextMetrics(hdc, &tm);
        int cxChar = tm.tmAveCharWidth;
        int cyChar = tm.tmHeight + tm.tmExternalLeading;
        // colors and stuff if we want
        /*ExtTextOut(hdc, 0 + m_padding, 0 + m_padding, 0, nullptr, m_healthText.c_str(), wcslen(m_healthText.c_str()),
                   nullptr);
        ExtTextOut(hdc, 0 + m_padding, cyChar + 20, 0, nullptr, m_healthEnemyOneText.c_str(),
                   wcslen(m_healthEnemyOneText.c_str()), nullptr);
        ExtTextOut(hdc, 0 + m_padding, cyChar + 50, 0, nullptr, m_healthEnemyTwoText.c_str(),
                   wcslen(m_healthEnemyTwoText.c_str()), nullptr);*/
        GetClientRect(m_hwnd, &rect);
        DrawText(hdc, L"Waiting for Resident Evil HD...", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        SetTextAlign(hdc, TA_CENTER | TA_TOP);
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        EndPaint(m_hwnd, &ps);
    }
        return 0;

    case WM_SYSCOLORCHANGE:
        InvalidateRect(m_hwnd, nullptr, TRUE);
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

auto UserInterface::CreateEditControls() -> void
{
    /*m_handleHealthPlayer =*/
    CreateWindowEx(WS_EX_STATICEDGE, L"Edit", L"Test123", /*WS_VISIBLE | */ WS_CHILD | SS_LEFT | WS_DISABLED, 200,
                   0 + m_padding, 100, 20, m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
    /*m_handleHealthEnemyOne =*/
    CreateWindowEx(WS_EX_STATICEDGE, L"Edit", 0, /*WS_VISIBLE | */ WS_CHILD | SS_LEFT | WS_DISABLED, 200,
                   30 + m_padding, 100, 20, m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
    /*m_handleHealthEnemyTwo =*/
    CreateWindowEx(WS_EX_STATICEDGE, L"Edit", 0, /*WS_VISIBLE | */ WS_CHILD | SS_LEFT | WS_DISABLED, 200,
                   60 + m_padding, 100, 20, m_hwnd, 0, (HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE), nullptr);
}
