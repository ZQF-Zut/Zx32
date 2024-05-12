#include <Wx32/Private/WxKernel32.h>

namespace Wx32::Kernel32
{
auto GetModuleFileNameU8(HMODULE hModule) -> MbcsStr_t
{
    uint32_t written_chars{};
    uint32_t buffer_max_chars = MAX_PATH;
    std::unique_ptr<wchar_t[]> buffer;
    do
    {
        buffer_max_chars *= 2;
        buffer = std::make_unique_for_overwrite<wchar_t[]>(buffer_max_chars);
        written_chars = ::GetModuleFileNameW(hModule, buffer.get(), buffer_max_chars);
    } while (written_chars >= buffer_max_chars);

    return Utils::ApiStrCvt({ buffer.get(), written_chars });
}

auto GetCurrentDirectoryU8() -> MbcsStr_t
{
    uint32_t real_chars{};
    uint32_t buffer_max_chars = MAX_PATH;
    std::unique_ptr<wchar_t[]> buffer;

    buffer = std::make_unique_for_overwrite<wchar_t[]>(buffer_max_chars);
    real_chars = ::GetCurrentDirectoryW(buffer_max_chars, buffer.get());

    if (real_chars > buffer_max_chars)
    {
        buffer_max_chars = real_chars;
        buffer = std::make_unique_for_overwrite<wchar_t[]>(buffer_max_chars);
        real_chars = ::GetCurrentDirectoryW(buffer_max_chars, buffer.get());
    }

    return Utils::ApiStrCvt({ buffer.get(), real_chars });
}

auto CreateFileU8(const std::string_view& u8FilePath, size_t dwDesiredAccess, size_t dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, size_t dwCreationDisposition, size_t dwFlagsAndAttributes, HANDLE hTemplateFile) -> std::optional<HANDLE>
{
    HANDLE hfile = ::CreateFileW(Utils::ApiStrCvt(u8FilePath).first.data(), static_cast<DWORD>(dwDesiredAccess), static_cast<DWORD>(dwShareMode), lpSecurityAttributes, static_cast<DWORD>(dwCreationDisposition), static_cast<DWORD>(dwFlagsAndAttributes), hTemplateFile);
    return (hfile != INVALID_HANDLE_VALUE) ? std::optional<HANDLE>{ hfile } : std::nullopt;
}

auto WriteFile(HANDLE hFile, std::span<const uint8_t> spData, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>
{
    DWORD written{};
    bool status = (::WriteFile(hFile, spData.data(), static_cast<DWORD>(spData.size()), &written, lpOverlapped) == TRUE);
    return status ? std::optional<size_t>{ static_cast<size_t>(written) } : std::nullopt;
}

auto ReadFile(HANDLE hFile, std::span<uint8_t> spBuffer, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>
{
    DWORD read{};
    bool status = (::ReadFile(hFile, spBuffer.data(), static_cast<DWORD>(spBuffer.size()), &read, lpOverlapped) == TRUE);
    return status ? std::optional<size_t>{ static_cast<size_t>(read) } : std::nullopt;
}

auto GetFileSizeEx(HANDLE hFile) -> std::optional<uint64_t>
{
    LARGE_INTEGER file_size{};
    ::GetFileSizeEx(hFile, &file_size);
    return static_cast<uint64_t>(file_size.QuadPart);
}

auto SetFilePointerEx(HANDLE hFile, uint64_t nOffset, size_t dwMoveMethod) -> std::optional<uint64_t>
{
    LARGE_INTEGER new_file_pointer;
    LARGE_INTEGER move_distance = { .QuadPart = static_cast<LONGLONG>((nOffset)) };
    bool status = (::SetFilePointerEx(hFile, move_distance, &new_file_pointer, static_cast<DWORD>(dwMoveMethod)) == TRUE);
    return status ? std::optional<uint64_t>{ static_cast<uint64_t>(new_file_pointer.QuadPart) } : std::nullopt;
}

auto CloseHandle(HANDLE hFile) -> bool
{
    return ::CloseHandle(hFile) == TRUE;
}

auto FlushFileBuffers(HANDLE hFile) -> bool
{
    return ::FlushFileBuffers(hFile) == TRUE;
}

auto CreateDirectoryU8(const std::string_view& u8DirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes) -> bool
{
    return ::CreateDirectoryW(Utils::ApiStrCvt(u8DirName).first.data(), lpSecurityAttributes) == TRUE;
}

auto RemoveDirectoryU8(const std::string_view& u8DirName) -> bool
{
    return ::RemoveDirectoryW(Utils::ApiStrCvt(u8DirName).first.data()) == TRUE;
}

auto GetFileAttributesU8(const std::string_view& u8Path) -> size_t
{
    return ::GetFileAttributesW(Utils::ApiStrCvt(u8Path).first.data());
}

auto DeleteFileU8(const std::string_view& u8FilePath) -> bool
{
    return ::DeleteFileW(Utils::ApiStrCvt(u8FilePath).first.data()) == TRUE;
}

auto CopyFileU8(const std::string_view& u8ExistFilePath, const std::string_view& u8NewFilePath, bool bFailIfExists) -> bool
{
    return ::CopyFileW(Utils::ApiStrCvt(u8ExistFilePath).first.data(), Utils::ApiStrCvt(u8NewFilePath).first.data(), bFailIfExists ? TRUE : FALSE) == TRUE;
}

auto MoveFileU8(const std::string_view& u8ExistFilePath, const std::string_view& u8NewFilePath) -> bool
{
    return ::MoveFileW(Utils::ApiStrCvt(u8ExistFilePath).first.data(), Utils::ApiStrCvt(u8NewFilePath).first.data()) == TRUE;
}

auto WriteConsoleU8(HANDLE hConsoleOutput, const std::string_view& u8Text, void* lpReserved) -> std::optional<size_t>
{
    DWORD written{};
    auto text_wstr = Utils::ApiStrCvt(u8Text);
    bool status = (::WriteConsoleW(hConsoleOutput, text_wstr.first.data(), static_cast<DWORD>(text_wstr.first.size()), &written, lpReserved) == TRUE);
    return status ? std::optional<size_t>{ static_cast<size_t>(written) } : std::nullopt;
}
} // namespace Wx32::Kernel32