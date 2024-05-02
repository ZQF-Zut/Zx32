#pragma once
#include <Wx32/Utils.h>

namespace Wx32::User32
{
auto SetWindowTextU8(HWND hWnd, const std::string_view& u8Text) -> bool;
auto MessageBoxU8(HWND hWnd, const std::string_view& u8Title, const std::string_view& u8Text, size_t uType) -> size_t;
} // namespace Wx32::User32