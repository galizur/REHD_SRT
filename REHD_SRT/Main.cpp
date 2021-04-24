#include "UserInterface.h"

auto WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) -> int
{
    UserInterface ui;
    /*Biohazard bhd;*/

    /*bhd.FindProcessHandle();
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
    }*/

    if (!ui.Create(L"ReHD SRT", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 325, 130))
    {
        return 0;
    }

    ShowWindow(ui.Window(), nCmdShow);

    MSG msg{};

    while (msg.message != WM_QUIT)
    {
        /*bhd.CalculatePlayersHealth();
        bhd.CalculateEnemiesHealth();*/

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}
