#include "Biohazard.h"

auto WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) -> int
{
    Biohazard bhd;

    bhd.FindProcessHandle();
    if (bhd.GetProcessHandle() == nullptr)
    {
        MessageBox(nullptr, L"Couldn't get process handle", bhd.ClassName(), MB_ICONERROR);
        return 0;
    }
    bhd.FindBaseAddress();
    if (bhd.GetBaseAddress() == 0)
    {
        MessageBox(nullptr, L"Couldn't find base address", bhd.ClassName(), MB_ICONERROR);
        return 0;
    }

    if (!bhd.Create(L"ReHD SRT", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 325, 130))
    {
        return 0;
    }

    ShowWindow(bhd.Window(), nCmdShow);

    MSG msg{};

    while (msg.message != WM_QUIT)
    {
        bhd.CalculatePlayersHealth();
        bhd.CalculateEnemiesHealth();
        // if (GetMessage(&msg, nullptr, 0, 0))
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        bhd.limiter.CalculateAndSleep();
    }

    return msg.wParam;
}
