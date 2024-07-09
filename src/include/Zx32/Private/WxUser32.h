#pragma once
#include <Zx32/Utils.h>

namespace Zx32::User32
{
    auto SetWindowTextU8(const std::string_view u8Text, WndID hWnd = WndID{ nullptr }) -> bool;
    auto MessageBoxU8(const std::string_view u8Title, const std::string_view u8Text, size_t uType, WndID hWnd = WndID{ nullptr }) -> size_t;
} // namespace Wx32::User32
