#pragma once

#include "BaseWindow.h"

#include <string>

class Biohazard
{
  public:
    // Memory stuff
    auto FindProcessHandle() -> void;
    auto FindBaseAddress() -> void;

    auto CalculatePlayersHealth() -> int;
    auto CalculateEnemiesHealth() -> std::vector<int>;

    auto GetProcessHandle() -> HANDLE;
    auto GetBaseAddress() const -> DWORD;

  private:
    std::wstring_view m_gameName{L"bhd.exe"};
    DWORD m_pid{0};
    HANDLE m_processHandle{nullptr};
    uintptr_t m_baseAddress{0};
    int m_healthPlayer{0};
    std::vector<int> m_healthEnemy{0, 0};
};

inline auto Biohazard::GetProcessHandle() -> HANDLE
{
    return m_processHandle;
}

inline auto Biohazard::GetBaseAddress() const -> DWORD
{
    return m_baseAddress;
}
