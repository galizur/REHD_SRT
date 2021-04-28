#include "UserInterface.h"

auto WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int nCmdShow) -> int
{
    auto ui{std::make_unique<UserInterface>()};

    if (!ui->Create(L"ReHD SRT", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 325, 130))
    {
        return 0;
    }

    // ShowWindow(ui->Window(), nCmdShow);

    std::thread processRetrieval(&UserInterface::RetrieveValues, ui.get());
    if (processRetrieval.joinable())
    {
        processRetrieval.detach();
    }

    MSG msg{};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return msg.wParam;
}
