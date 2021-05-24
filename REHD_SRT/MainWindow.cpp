#include "MainWindow.h"

#include <string>

static const int windowWidth{500};
static const int windowHeight{700};

MainWindow::MainWindow(HINSTANCE hInstance, int nShowCmd) : m_hInstance{hInstance}, m_nShowCmd{nShowCmd}
{
}

MainWindow::~MainWindow()
{
    if (m_healthFineBitmap)
    {
        m_healthFineBitmap.release();
    }
    if (m_healthCautionBitmap)
    {
        m_healthCautionBitmap.release();
    }
}

auto MainWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    MainWindow *mainWindow{InstanceFromWndProc<MainWindow, MainWindow, &MainWindow::m_hwnd>(hWnd, uMsg, lParam)};

    // The first WM_GETMINMAXINFO comes before WM_NCCREATE, before we get our MainWindow pointer.
    if (mainWindow)
    {
        switch (uMsg)
        {
        case WM_CREATE:
            return mainWindow->OnCreate();
        case WM_DESTROY:
            return mainWindow->OnDestroy();
        case WM_DPICHANGED:
            return mainWindow->OnDpiChange(wParam, lParam);
        case WM_GETMINMAXINFO:
            return mainWindow->OnGetMinMaxInfo(lParam);
        case WM_PAINT:
            return mainWindow->OnPaint();
        case WM_SIZE:
            return mainWindow->OnSize();
        case WM_DRAWITEM:
            return mainWindow->OnDrawItem();
        }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

auto MainWindow::Create(HINSTANCE hInstance, int nShowCmd) -> std::unique_ptr<MainWindow>
{
    // Register the window class
    WNDCLASSW wc{0};
    wc.lpfnWndProc = MainWindow::WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    // wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON));
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszClassName = MainWindow::wndClass;

    if (!RegisterClassW(&wc))
    {
        std::wstring message{L"RegisterClassW failed, last error is " + std::to_wstring(GetLastError())};
        MessageBoxW(nullptr, message.c_str(), L"Error", MB_ICONERROR);
        return nullptr;
    }

    // Create the main window
    auto mainWindow = std::unique_ptr<MainWindow>(new MainWindow(hInstance, nShowCmd));
    HWND hWnd{CreateWindowExW(0, MainWindow::wndClass, appName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance,
                              mainWindow.get())};

    if (!hWnd)
    {
        std::wstring message{L"CreateWindowExW failed, last error is " + std::to_wstring(GetLastError())};
        MessageBoxW(nullptr, message.c_str(), L"Error", MB_ICONERROR);
        return nullptr;
    }

    return mainWindow;
}

auto MainWindow::WorkLoop() -> int
{
    MSG msg;
    for (;;)
    {
        BOOL ret{GetMessageW(&msg, 0, 0, 0)};
        if (ret > 0)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else if (ret == 0)
        {
            // WM_QUIT message terminated the message loop
            return msg.wParam;
        }
        else
        {
            std::wstring message{L"GetMessageW failed, last error is " + std::to_wstring(GetLastError())};
            MessageBoxW(nullptr, message.c_str(), L"Error", MB_ICONERROR);
            return 1;
        }
    }
}

auto MainWindow::OnCreate() -> LRESULT
{
    // Get the DPI setting for the monitor where the main window is shown.
    m_currentDpi = GetWindowDpi(m_hwnd);

    // Load resources
    m_healthFineBitmap = LoadGIFAsGdiplusBitmap(m_hInstance, IDG_HEALTH_FINE);
    m_healthCautionBitmap = LoadGIFAsGdiplusBitmap(m_hInstance, IDG_HEALTH_CAUTION1);

    // Create the main GUI font.
    wcscpy_s(m_lfGuiFont.lfFaceName, L"Courier New Bold");
    // m_lfGuiFont.lfWeight = 900;
    m_lfGuiFont.lfHeight = -MulDiv(12, m_currentDpi, fontReferenceDPI);
    m_guiFont = CreateFontIndirectW(&m_lfGuiFont);

    // Create controls

    // Visible
    /*m_waitingText = CreateWindowExW(0, WC_STATICW, L"Waiting for Biohazard HD Remaster...",
                                    WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, m_hwnd, nullptr, nullptr,
      nullptr);*/

    // Invisible
    m_waitingText = CreateWindowExW(0, WC_STATICW, L"Waiting for Biohazard HD Remaster...", WS_CHILD | SS_CENTER, 0, 0,
                                    0, 0, m_hwnd, nullptr, nullptr, nullptr);
    SendMessageW(m_waitingText, WM_SETFONT, reinterpret_cast<WPARAM>(m_guiFont), MAKELPARAM(TRUE, 0));

    // TEST
    m_enemyHealthBar = CreateWindowExW(0, PROGRESS_CLASSW, (LPTSTR)NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTHREVERSE, 0,
                                       0, 0, 0, m_hwnd, nullptr, nullptr, nullptr);

    SetWindowTheme(m_enemyHealthBar, L"", L"");
    SendMessage(m_enemyHealthBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessage(m_enemyHealthBar, PBM_SETPOS, (WPARAM)100, 0);
    SendMessage(m_enemyHealthBar, PBM_SETBARCOLOR, 0, (LPARAM)RGB(138, 3, 3));

    // Set the main window size.
    int height = MulDiv(windowHeight, m_currentDpi, windowsReferenceDPI);
    int width = MulDiv(windowWidth, m_currentDpi, windowsReferenceDPI);
    SetWindowPos(m_hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE);

    // Show the window.
    ShowWindow(m_hwnd, m_nShowCmd);

    return 0;
}

auto MainWindow::OnGetMinMaxInfo(LPARAM lParam) -> LRESULT
{
    PMINMAXINFO minMaxInfo{reinterpret_cast<PMINMAXINFO>(lParam)};

    minMaxInfo->ptMinTrackSize.x = MulDiv(windowWidth, m_currentDpi, windowsReferenceDPI);
    minMaxInfo->ptMinTrackSize.y = MulDiv(windowHeight, m_currentDpi, windowsReferenceDPI);

    return 0;
}

auto MainWindow::OnDestroy() -> LRESULT
{
    PostQuitMessage(0);
    return 0;
}

auto MainWindow::OnDpiChange(WPARAM wParam, LPARAM lParam) -> LRESULT
{
    m_currentDpi = LOWORD(wParam);

    // Redraw the entire window on every DPI change.
    RECT rcWindow;
    GetClientRect(m_hwnd, &rcWindow);
    InvalidateRect(m_hwnd, &rcWindow, FALSE);

    // Recalculate the main GUI font.
    LOGFONTW lfNewGuiFont{m_lfGuiFont};
    lfNewGuiFont.lfHeight = -MulDiv(12, m_currentDpi, fontReferenceDPI);
    HFONT newGuiFont{CreateFontIndirectW(&lfNewGuiFont)};
    DeleteObject(m_guiFont);
    m_guiFont = newGuiFont;

    // Update the control fonts.
    SendMessageW(m_waitingText, WM_SETFONT, reinterpret_cast<WPARAM>(m_guiFont), MAKELPARAM(TRUE, 0));

    // Use the suggested new window size.
    RECT *const newWindow{reinterpret_cast<RECT *>(lParam)};
    int windowX{newWindow->left};
    int windowY{newWindow->top};
    int windowWidth{newWindow->right - newWindow->left};
    int windowHeight{newWindow->bottom - newWindow->top};
    SetWindowPos(m_hwnd, nullptr, windowX, windowY, windowWidth, windowHeight, SWP_NOZORDER | SWP_NOACTIVATE);

    return 0;
}

auto MainWindow::OnPaint() -> LRESULT
{
    // Get the window size.
    RECT rcWindow;
    GetClientRect(m_hwnd, &rcWindow);

    // Begin a double buffered paint.
    PAINTSTRUCT ps;
    HDC hdc{BeginPaint(m_hwnd, &ps)};
    HDC hMemDC{CreateCompatibleDC(hdc)};
    HBITMAP hMemBitmap{CreateCompatibleBitmap(hdc, rcWindow.right, rcWindow.bottom)};
    SelectObject(hMemDC, hMemBitmap);

    // Fill the window with the window background color.
    RECT rcBackground{rcWindow};
    FillRect(hMemDC, &rcBackground, GetSysColorBrush(COLOR_BTNFACE));

    // Draw Health in top left corner
    int destBitmapHeight{MulDiv(m_healthFineBitmap->GetHeight(), m_currentDpi, windowsReferenceDPI)};
    int destBitmapWidth{MulDiv(m_healthFineBitmap->GetWidth(), m_currentDpi, windowsReferenceDPI)};
    int destBitmapX{rcWindow.top};
    int destBitmapY{rcWindow.left};

    Gdiplus::Graphics g(hMemDC);
    g.DrawImage(m_healthFineBitmap.get(), destBitmapX, destBitmapY, destBitmapWidth, destBitmapHeight);

    // End painting by copying the in-memory DC.
    BitBlt(hdc, 0, 0, rcWindow.right, rcWindow.bottom, hMemDC, 0, 0, SRCCOPY);
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);
    EndPaint(m_hwnd, &ps);

    return 0;
}

auto MainWindow::OnSize() -> LRESULT
{
    // Get the window size.
    RECT rcWindow;
    GetClientRect(m_hwnd, &rcWindow);

    // Move and redraw stuff.
    HDWP hDwp{BeginDeferWindowPos(3)};
    if (!hDwp)
    {
        return 0;
    }

    const int controlPadding{MulDiv(m_controllPadding, m_currentDpi, windowsReferenceDPI)};
    const int waitingTextHeight{MulDiv(m_waitingTextHeight, m_currentDpi, windowsReferenceDPI)};
    const int waitingTextWidth{MulDiv(m_waitingTextWidth, m_currentDpi, windowsReferenceDPI)};
    int waitingTextX{rcWindow.right / 2 - controlPadding / 2 - waitingTextWidth / 2};
    int waitingTextY{rcWindow.bottom / 2 - controlPadding / 2 - waitingTextHeight / 2};
    hDwp = DeferWindowPos(hDwp, m_waitingText, nullptr, waitingTextX, waitingTextY, waitingTextWidth, waitingTextHeight,
                          0);

    if (!hDwp)
    {
        return 0;
    }

    const int enemyHealthBarHeight{MulDiv(15, m_currentDpi, windowsReferenceDPI)};
    const int enemyHealthBarWidth{MulDiv(150, m_currentDpi, windowsReferenceDPI)};
    const int enemyHealthBarX{0};
    const int enemyHealthBarY{MulDiv(70, m_currentDpi, windowsReferenceDPI)};
    hDwp = DeferWindowPos(hDwp, m_enemyHealthBar, nullptr, enemyHealthBarX, enemyHealthBarY, enemyHealthBarWidth,
                          enemyHealthBarHeight, 0);
    if (!hDwp)
    {
        return 0;
    }

    EndDeferWindowPos(hDwp);

    return 0;
}

auto MainWindow::OnDrawItem() -> LRESULT
{
    return LRESULT();
}
