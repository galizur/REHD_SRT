#pragma once

#include "BaseWindow.h"

#include "Biohazard.h"

class UserInterface : public BaseWindow<UserInterface>
{
  public:
    UserInterface();
    virtual ~UserInterface() = default;

    auto ClassName() const -> PCWSTR override
    {
        return L"BaseUI";
    }
    auto HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;

    auto RetrieveValues() -> void;
    auto CalculateHealthValues() -> void;

  private:
    auto GetWindowDPI(HWND hwnd) -> WORD;
    auto SetColors(HDC hdc, PAINTSTRUCT *ps) -> void;
    auto CreateStaticControls(HDC hdc) -> void;
    auto CreateEditControls() -> void;
    auto CreateUserFont(HDC hdc) -> HFONT;
    auto LoadStringAsWstr(UINT uID) -> std::wstring;
    auto LoadStringAsWstr(HINSTANCE hInstance, UINT uID) -> std::wstring;
    auto DrawWaitingText(HDC hdc) -> void;

    // Drawing variables
    int m_currentDPI{96};
    int m_editPositionX{0};
    int m_padding{5};
    LOGFONTW m_lfguiFont{0};
    HFONT m_guiFont{nullptr};
    TEXTMETRIC m_tm{};
    int m_cxChar{0};
    int m_cyChar{0};
    // Biohazard variables
    std::unique_ptr<Biohazard> m_bhd;
    bool m_readyToShow{false};
    int m_pHealth{0};
    std::vector<int> m_eHealth{0};
    HWND m_hHealthPlayer{nullptr};
    HWND m_hHealthEnemyOne{nullptr};
    HWND m_hHealthEnemyTwo{nullptr};
};
