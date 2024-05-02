#include <Wx32/Utils.h>
#include <span>
#include <stdexcept>

namespace Wx32::Utils
{
auto PathStrCvtU8ToU16(const std::string_view& u8Str, U16PathBuffer& sfU16Buffer) -> std::wstring_view
{
    std::span<wchar_t> sp_buffer = sfU16Buffer.AccessOnce();

    if (u8Str.empty())
    {
        sp_buffer[0] = L'\0';
        return { L"", 0 };
    }

    const auto char_count = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, u8Str.data(), static_cast<int>(u8Str.size()), nullptr, 0));
    if (char_count > sp_buffer.size())
    {
        std::runtime_error("path too long");
    }

    if (char_count == 0)
    {
        sp_buffer[0] = L'\0';
        return { L"", 0 };
    }

    const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, u8Str.data(), static_cast<int>(u8Str.size()), sp_buffer.data(), static_cast<int>(char_count)));
    sp_buffer[char_count_real] = L'\0';
    return { sp_buffer.data(), char_count_real };
}

auto PathStrCvtU16ToU8(const std::wstring_view& u16Str, U8PathBuffer& sfU8Buffer) -> std::string_view
{
    std::span<char> sp_buffer = sfU8Buffer.AccessOnce();

    if (u16Str.empty())
    {
        sp_buffer[0] = '\0';
        return { "", 0 };
    }

    if (u16Str.size() > MAX_PATH)
    {
        std::runtime_error("path too long");
    }

    const auto bytes = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, u16Str.data(), static_cast<int>(u16Str.size()), nullptr, 0, nullptr, nullptr));

    if (bytes == 0 || bytes > sp_buffer.size())
    {
        sp_buffer[0] = '\0';
        return { "", 0 };
    }

    const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(u16Str.data()), static_cast<int>(u16Str.size()), sp_buffer.data(), static_cast<int>(bytes), nullptr, nullptr));
    sp_buffer[bytes_real] = '\0';
    return { sp_buffer.data(), bytes_real };
}

using WideStr_t = std::pair<std::wstring_view, std::unique_ptr<wchar_t[]>>;
auto StrCvtU8ToU16(const std::string_view& u8Str) -> WideStr_t
{
    if (u8Str.empty()) { return { std::wstring_view{ L"", 0 }, nullptr }; }
    const auto char_count = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, u8Str.data(), static_cast<int>(u8Str.size()), nullptr, 0));
    if (char_count == 0) { return { std::wstring_view{ L"", 0 }, nullptr }; }
    auto buffer = std::make_unique_for_overwrite<wchar_t[]>(char_count);
    const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, u8Str.data(), static_cast<int>(u8Str.size()), buffer.get(), static_cast<int>(char_count)));
    buffer[char_count_real] = L'\0';
    return { std::wstring_view{ buffer.get(), char_count_real }, std::move(buffer) };
}

using MbcsStr_t = std::pair<std::string_view, std::unique_ptr<char[]>>;
auto StrCvtU16ToU8(const std::wstring_view& u16Str) -> MbcsStr_t
{
    if (u16Str.empty()) { return { std::string_view{ "", 0 }, nullptr }; }
    const auto bytes = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, u16Str.data(), static_cast<int>(u16Str.size()), nullptr, 0, nullptr, nullptr));
    if (bytes == 0) { return { std::string_view{ "", 0 }, nullptr }; }
    auto buffer = std::make_unique_for_overwrite<char[]>(bytes);
    const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(u16Str.data()), static_cast<int>(u16Str.size()), buffer.get(), static_cast<int>(bytes), nullptr, nullptr));
    buffer[bytes_real] = '\0';
    return { std::string_view{ buffer.get(), bytes_real }, std::move(buffer) };
}
} // namespace Wx32::Utils