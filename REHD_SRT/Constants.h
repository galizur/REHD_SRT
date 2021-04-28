#pragma once

#include <cstdint>

namespace offset
{
constexpr std::uintptr_t base{0x009E41BC};
namespace health
{
constexpr std::uintptr_t playerFirst{0x0000014C};
constexpr std::uintptr_t playerSecond{0x000013BC};
constexpr std::uintptr_t enemyOneFirst{0x00000194};
constexpr std::uintptr_t enemyOneSecond{0x000013BC};
constexpr std::uintptr_t enemyTwoFirst{0x00000198};
constexpr std::uintptr_t enemyTwoSecond{0x000013BC};
} // namespace health
} // namespace offset

namespace dimension
{
constexpr int width96{325};
constexpr int height96{130};
} // namespace dimension
