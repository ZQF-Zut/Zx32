#include <Zx32/Utils.h>
#include <stdexcept>


namespace Zx32::Utils
{
    auto StrCvtForce(const std::string_view msStr, const CodePage eCodePage) -> WideStr_t
    {
        if (msStr.empty()) { return { std::wstring_view{ L"", 0 }, nullptr }; }
        const auto char_count = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), nullptr, 0));
        if (char_count == 0) { return { std::wstring_view{ L"", 0 }, nullptr }; }
        auto buffer = std::make_unique_for_overwrite<wchar_t[]>(char_count + 1);
        const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), buffer.get(), static_cast<int>(char_count)));
        buffer[char_count_real] = {};
        return { std::wstring_view{ buffer.get(), char_count_real }, std::move(buffer) };
    }

    auto StrCvtForce(const std::wstring_view wsStr, const CodePage eCodePage) -> MbcsStr_t
    {
        if (wsStr.empty()) { return { std::string_view{ "", 0 }, nullptr }; }
        const auto bytes = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, wsStr.data(), static_cast<int>(wsStr.size()), nullptr, 0, nullptr, nullptr));
        if (bytes == 0) { return { std::string_view{ "", 0 }, nullptr }; }
        auto buffer = std::make_unique_for_overwrite<char[]>(bytes + 1);
        const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, reinterpret_cast<const wchar_t*>(wsStr.data()), static_cast<int>(wsStr.size()), buffer.get(), static_cast<int>(bytes), nullptr, nullptr));
        buffer[bytes_real] = {};
        return { std::string_view{ buffer.get(), bytes_real }, std::move(buffer) };
    }

    auto StrCvtForce(const std::string_view msStr, std::span<wchar_t> spBuffer, const CodePage eCodePage) -> std::wstring_view
    {
        if (msStr.empty()) { return { L"", 0 }; }
        const auto char_count = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), spBuffer.data(), static_cast<int>(spBuffer.size())));
        if ((char_count == 0) || (char_count >= spBuffer.size())) { throw std::runtime_error("ApiStrCvt: buffer too small"); }
        spBuffer[char_count] = {};
        return { spBuffer.data(), char_count };
    }

    auto StrCvtForce(const std::wstring_view wsStr, std::span<char> spBuffer, const CodePage eCodePage) -> std::string_view
    {
        if (wsStr.empty()) { return { "", 0 }; }
        const auto bytes = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, reinterpret_cast<const wchar_t*>(wsStr.data()), static_cast<int>(wsStr.size()), spBuffer.data(), static_cast<int>(spBuffer.size()), nullptr, nullptr));
        if ((bytes == 0) || (bytes >= spBuffer.size())) { throw std::runtime_error("ApiStrCvt: buffer too small"); }
        spBuffer[bytes] = {};
        return { spBuffer.data(), bytes };
    }

    auto StrCvtSafe(const std::wstring_view wsStr, const CodePage eCodePage) -> MbcsStr_t
    {
        // warning log func, print error msg/text to console via sys api
        auto fn_warning_log = [&wsStr](std::wstring_view const wsReason) {
            DWORD written{};
            const HANDLE handle = ::GetStdHandle(STD_ERROR_HANDLE);
            if (handle == INVALID_HANDLE_VALUE) { return; }
            ::WriteConsoleW(handle, wsReason.data(), static_cast<DWORD>(wsReason.size()), &written, nullptr);
            ::WriteConsoleW(handle, L"---> ", 5, &written, nullptr);
            ::WriteConsoleW(handle, wsStr.data(), static_cast<DWORD>(wsStr.size()), &written, nullptr);
            ::WriteConsoleW(handle, L"\n\n", 2, &written, nullptr);
            };

        // if src str is empty just return
        if (wsStr.empty())
        {
            // let std::string_view reference the global string(null char string), avoiding memory allocation
            return { std::string_view{ "", 0 }, nullptr };
        }

        // flag for if missing chars
        BOOL not_all_cvt = TRUE;

        // try count multi-bytes string bytes (exclude null char)
        const auto bytes = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, wsStr.data(), static_cast<int>(wsStr.size()), nullptr, 0, nullptr, &not_all_cvt));

        // if char count == 0 that means the cvt failed
        if (bytes == 0)
        {
            fn_warning_log(L"[Warning] StrCvtSafe: convert WideChar to MBCS error!\n");

            // let std::string_view reference the global string(null char string), avoiding memory allocation
            return { std::string_view{ "", 0 }, nullptr };
        }

        // if missing chars show a msg
        if (not_all_cvt == TRUE)
        {
            fn_warning_log(L"[Warning] StrCvtSafe: convert WideChar to MBCS missing chars!\n");
        }

        // make unique ptr but not initialize the buffer
        auto buffer_ptr = std::make_unique_for_overwrite<char[]>(bytes + 1);

        // write data to buffer
        const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(static_cast<UINT>(eCodePage), 0, reinterpret_cast<const wchar_t*>(wsStr.data()), static_cast<int>(wsStr.size()), buffer_ptr.get(), static_cast<int>(bytes), nullptr, &not_all_cvt));

        // add null char to the end
        buffer_ptr.get()[bytes_real] = {};

        // return pair
        return { std::string_view{ buffer_ptr.get(), bytes_real }, std::move(buffer_ptr) };
    }

    auto StrCvtSafe(const std::string_view msStr, const CodePage eCodePage) -> WideStr_t
    {
        // warning log func, print error msg/text to console via sys api
        auto fn_warning_log = [&msStr](std::string_view const msReason) {
            DWORD written{};
            const HANDLE handle = ::GetStdHandle(STD_ERROR_HANDLE);
            if (handle == INVALID_HANDLE_VALUE) { return; }
            ::WriteConsoleA(handle, msReason.data(), static_cast<DWORD>(msReason.size()), &written, nullptr);
            ::WriteConsoleW(handle, L"---> ", 5, &written, nullptr);
            ::WriteConsoleA(handle, msStr.data(), static_cast<DWORD>(msStr.size()), &written, nullptr);
            ::WriteConsoleW(handle, L"\n\n", 2, &written, nullptr);
            };

        // if src str is empty just return
        if (msStr.empty())
        {
            // let std::wstring_view reference the global string(null char string), avoiding memory allocation
            return { std::wstring_view{ L"", 0 }, nullptr };
        }

        // try count widechar string chars (exclude null char)
        const auto char_count = static_cast<size_t>(::MultiByteToWideChar(static_cast<UINT>(eCodePage), MB_ERR_INVALID_CHARS, msStr.data(), static_cast<int>(msStr.size()), nullptr, 0));

        // if char count == 0 that means the conversion failed
        if (char_count == 0)
        {
            // show some information for user
            fn_warning_log("[Warning] StrCvtSafe: convert MBCS to WideChar error!\n");

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

    auto ApiStrCvt(const std::wstring_view u16Str) -> MbcsStr_t
    {
        return StrCvtForce(u16Str, CodePage::UTF8);
    }

    auto ApiStrCvt(const std::string_view u8Str) -> WideStr_t
    {
        return StrCvtForce(u8Str, CodePage::UTF8);
    }

    auto ApiStrCvt(const std::string_view u8Str, std::span<wchar_t> spBuffer) -> std::wstring_view
    {
        return StrCvtForce(u8Str, spBuffer, CodePage::UTF8);
    }

    auto ApiStrCvt(const std::wstring_view u16Str, std::span<char> spBuffer) -> std::string_view
    {
        return StrCvtForce(u16Str, spBuffer, CodePage::UTF8);
    }

    auto ForceU8Str(const std::u8string_view msStr) -> std::string_view
    {
        return { reinterpret_cast<const char*>(msStr.data()), msStr.size() };
    }
} // namespace Wx32::Utils
