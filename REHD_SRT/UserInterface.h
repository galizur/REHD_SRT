#pragma once

#include "BaseWindow.h"

#include "Biohazard.h"

class UserInterface : public BaseWindow<UserInterface>
{
  public:
    auto ClassName() const -> PCWSTR override
    {
        return L"BaseUI";
    }
    auto HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT override;

  private:
    auto CreateEditControls() -> void;

    Biohazard bhd;
    int m_padding{5};
};
