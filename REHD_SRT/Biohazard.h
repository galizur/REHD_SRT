#pragma once

#include "BaseWindow.h"

#include <string>

class Biohazard /*: public BaseWindow<Biohazard>*/
{
  public:
    /*auto ClassName() const -> PCWSTR override
    {
        return L"ReHD_SRT";
    }
    auto HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;*/

    // Memory stuff
    auto FindProcessHandle() -> void;
    auto FindBaseAddress() -> void;

    auto CalculatePlayersHealth() -> int;
    auto CalculateEnemiesHealth() -> std::vector<int>;

    auto GetProcessHandle() -> HANDLE;
    auto GetBaseAddress() const -> DWORD;

  private:
    std::wstring m_healthText{L"Players health:"};
    std::wstring m_healthEnemyOneText{L"First enemy's health:"};
    std::wstring m_healthEnemyTwoText{L"Second enemy's health:"};

    /*HWND m_handleHealthPlayer{nullptr};
    HWND m_handleHealthEnemyOne{nullptr};
    HWND m_handleHealthEnemyTwo{nullptr};*/

    std::wstring_view m_gameName{L"bhd.exe"};
    DWORD m_pid{0};
    HANDLE m_processHandle{nullptr};
    uintptr_t m_baseAddress{0}; // Was DWORD
    int m_healthPlayer{0};
    std::vector<int> m_healthEnemy{0, 0};
    /*int m_healthEnemyOne{0};
    int m_healthEnemyTwo{0};*/
    std::mutex m_pHealthMut;
    std::mutex m_eHealthMut;
};

inline auto Biohazard::GetProcessHandle() -> HANDLE
{
    return m_processHandle;
}

inline auto Biohazard::GetBaseAddress() const -> DWORD
{
    return m_baseAddress;
}
