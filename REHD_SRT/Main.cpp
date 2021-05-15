#include "Framework.h"

#include "MainWindow.h"

auto WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine,
                     _In_ int nShowCmd) -> int
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize GDI+
    Gdiplus::GdiplusStartupInput startupInput;
    ULONG_PTR token;
    Gdiplus::GdiplusStartup(&token, &startupInput, nullptr);

    // Initialize standard controls
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES | ICC_ANIMATE_CLASS;
    InitCommonControlsEx(&icc);

    int returnValue{1};

    auto mainWindow{MainWindow::Create(hInstance, nShowCmd)};
    if (mainWindow)
    {
        returnValue = mainWindow->WorkLoop();
    }

    // Cleanup
    Gdiplus::GdiplusShutdown(token);

    return returnValue;
}
