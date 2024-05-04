#include <Wx32/Utils.h>
#include <span>
#include <stdexcept>

namespace Wx32::Utils
{
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

auto StrCvtToMbcs(const std::wstring_view& wsStr, const CodePage eCodePage) noexcept -> MbcsStr_t
{
    auto fn_warning_log = [&wsStr](const std::wstring_view& wsReason) {
        DWORD written{};
        const HANDLE handle = ::GetStdHandle(STD_ERROR_HANDLE);
        ::WriteConsoleW(handle, wsReason.data(), static_cast<DWORD>(wsReason.size()), &written, nullptr);
        ::WriteConsoleW(handle, L"---> ", 5, &written, nullptr);
        ::WriteConsoleW(handle, wsStr.data(), static_cast<DWORD>(wsStr.size()), &written, nullptr);
        ::WriteConsoleW(handle, L"\n\n", 2, &written, nullptr);
    };

    if (wsStr.empty())
    {
        return { std::string_view{ "", 0 }, nullptr };
    }

    BOOL not_all_cvt = TRUE;
    const auto bytes = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, wsStr.data(), static_cast<int>(wsStr.size()), nullptr, 0, nullptr, &not_all_cvt));

    if (bytes == 0)
    {
        fn_warning_log(L"[Warning] MBCStr: convert WideChar to MBCS error!\n");
        return { std::string_view{ "", 0 }, nullptr };
    }

    if (not_all_cvt == TRUE)
    {
        fn_warning_log(
            L"[Warning] MBCStr: convert WideChar to MBCS missing char!\n");
    }

    auto buffer_ptr = std::make_unique_for_overwrite<char[]>(bytes + 1);
    const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, reinterpret_cast<const wchar_t*>(wsStr.data()), static_cast<int>(wsStr.size()), buffer_ptr.get(), static_cast<int>(bytes), nullptr, &not_all_cvt));
    buffer_ptr.get()[bytes_real] = {};
    return { std::string_view{ buffer_ptr.get(), bytes_real }, std::move(buffer_ptr) };
}

auto StrCvtToWide(const std::string_view& msStr, const CodePage eCodePage) noexcept -> WideStr_t
{
    // warning log func, print error msg/text to console via sys api
    auto fn_warning_log = [&msStr](const std::string_view& msReason) {
        DWORD written{};
        const HANDLE handle = ::GetStdHandle(STD_ERROR_HANDLE);
        ::WriteConsoleA(handle, msReason.data(), static_cast<DWORD>(msReason.size()), &written, nullptr);
        ::WriteConsoleW(handle, L"---> ", 5, &written, nullptr);
        ::WriteConsoleA(handle, msStr.data(), static_cast<DWORD>(msStr.size()), &written, nullptr);
        ::WriteConsoleW(handle, L"\n\n", 2, &written, nullptr);
    };

    // if src str is empty just return
    if (msStr.empty())
    {
        // let std::wstring_view reference the global string(null char string),
        // avoiding Memory allocation
        return { std::wstring_view{ L"", 0 }, nullptr };
    }

    // try count widechar string chars (exclude null char)
    const auto char_count = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), nullptr, 0));

    // if char count == 0 that means the conversion failed
    if (char_count == 0)
    {
        // show some information for user
        fn_warning_log("[Warning] WideStr: convert MBCS to WideChar error!\n");

        // again avoiding Memory allocation
        return { std::wstring_view{ L"", 0 }, nullptr };
    }

    // make unique ptr but not initialize the buffer
    auto buffer_ptr = std::make_unique_for_overwrite<wchar_t[]>(char_count + 1);

    // write data to buffer
    const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), buffer_ptr.get(), static_cast<int>(char_count)));

    // add null char to the end
    buffer_ptr.get()[char_count_real] = {};

    // return pair
    return { std::wstring_view{ buffer_ptr.get(), char_count_real }, std::move(buffer_ptr) };
}

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
} // namespace Wx32::Utils