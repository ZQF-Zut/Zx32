#include <Wx32/Private/WxUser32.h>

namespace Wx32::User32
{
auto SetWindowTextU8(HWND hWnd, const std::string_view& u8Text) -> bool
{
    return ::SetWindowTextW(hWnd, Utils::StrCvtU8ToU16(u8Text).first.data()) == TRUE;
}

auto MessageBoxU8(HWND hWnd, const std::string_view& u8Title, const std::string_view& u8Text, size_t uType) -> size_t
{
    return static_cast<size_t>(::MessageBoxW(hWnd, Utils::StrCvtU8ToU16(u8Text).first.data(), Utils::StrCvtU8ToU16(u8Title).first.data(), static_cast<UINT>(uType)));
}
} // namespace Wx32::User32