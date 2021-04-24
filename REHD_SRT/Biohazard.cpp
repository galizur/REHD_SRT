#include "Biohazard.h"

#include <TlHelp32.h>
#include <process.h>

constexpr uintptr_t BASE_OFFSET{0x009E41BC};
constexpr uintptr_t HEALTH_OFFSET_FIRST{0x0000014C};
constexpr uintptr_t HEALTH_OFFSET_SECOND{0x000013BC};
constexpr DWORD ENEMY_ONE_OFFSET_FIRST{0x00000194};
constexpr DWORD ENEMY_ONE_OFFSET_SECOND{0x000013BC};
constexpr DWORD ENEMY_TWO_OFFSET_FIRST{0x00000198};
constexpr DWORD ENEMY_TWO_OFFSET_SECOND{0x000013BC};

// auto Biohazard::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
//{
//
//}

auto Biohazard::FindProcessHandle() -> void
{
    HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)};
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }
    PROCESSENTRY32 process{0};
    SecureZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &process))
    {
        do
        {
            if (std::wstring(process.szExeFile) == m_gameName)
            {
                m_pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    if (m_pid == 0)
    {
        return;
    }
    // m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, m_pid);
    m_processHandle = OpenProcess(PROCESS_VM_READ, false, m_pid);
    CloseHandle(snapshot);
}

auto Biohazard::FindBaseAddress() -> void
{
    HANDLE snapshot{CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pid)};
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    MODULEENTRY32 module{0};
    SecureZeroMemory(&module, sizeof(module));
    module.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &module))
    {
        do
        {
            if (std::wstring(module.szModule) == m_gameName)
            {
                m_baseAddress = (uintptr_t)module.modBaseAddr;
                break;
            }
        } while (Module32Next(snapshot, &module));
    }

    if (m_baseAddress == 0)
    {
        return;
    }

    CloseHandle(snapshot);
}

auto Biohazard::CalculatePlayersHealth() -> void
{
    uintptr_t addr{BASE_OFFSET + m_baseAddress};
    uintptr_t health;
    auto rr = ReadProcessMemory(m_processHandle, (BYTE *)addr, &health, sizeof(health), nullptr);
    health += HEALTH_OFFSET_FIRST;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)health, &health, sizeof(health), nullptr);
    health += HEALTH_OFFSET_SECOND;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)health, &health, sizeof(health), nullptr);

    if (m_healthPlayer != health)
    {
        m_healthPlayer = health;
        SetWindowText(m_handleHealthPlayer, std::to_wstring(m_healthPlayer).c_str());
        /*auto r = SendMessage(m_handleHealthPlayer, EN_CHANGE, 0, 0);*/
        InvalidateRect(m_handleHealthPlayer, nullptr, TRUE);
        UpdateWindow(m_handleHealthPlayer);
    }
}

auto Biohazard::CalculateEnemiesHealth() -> void
{
    uintptr_t addr{BASE_OFFSET + m_baseAddress};
    uintptr_t enemyOne;
    auto rr = ReadProcessMemory(m_processHandle, (BYTE *)addr, &enemyOne, sizeof(enemyOne), nullptr);
    enemyOne += ENEMY_ONE_OFFSET_FIRST;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)enemyOne, &enemyOne, sizeof(enemyOne), nullptr);
    enemyOne += ENEMY_ONE_OFFSET_SECOND;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)enemyOne, &enemyOne, sizeof(enemyOne), nullptr);

    if (m_healthEnemyOne != enemyOne)
    {
        m_healthEnemyOne = enemyOne;
        SetWindowText(m_handleHealthEnemyOne, std::to_wstring(m_healthEnemyOne).c_str());
        /*auto r = SendMessage(m_handleHealthPlayer, EN_CHANGE, 0, 0);*/
        InvalidateRect(m_handleHealthEnemyOne, nullptr, TRUE);
        UpdateWindow(m_handleHealthEnemyOne);
    }

    addr = BASE_OFFSET + m_baseAddress;
    uintptr_t enemyTwo;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)addr, &enemyTwo, sizeof(enemyTwo), nullptr);
    enemyTwo += ENEMY_TWO_OFFSET_FIRST;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)enemyTwo, &enemyTwo, sizeof(enemyTwo), nullptr);
    enemyTwo += ENEMY_TWO_OFFSET_SECOND;
    rr = ReadProcessMemory(m_processHandle, (BYTE *)enemyTwo, &enemyTwo, sizeof(enemyTwo), nullptr);

    if (m_healthEnemyTwo != enemyTwo)
    {
        m_healthEnemyTwo = enemyTwo;
        SetWindowText(m_handleHealthEnemyTwo, std::to_wstring(m_healthEnemyTwo).c_str());
        /*auto r = SendMessage(m_handleHealthPlayer, EN_CHANGE, 0, 0);*/
        InvalidateRect(m_handleHealthEnemyTwo, nullptr, TRUE);
        UpdateWindow(m_handleHealthEnemyTwo);
    }
}
