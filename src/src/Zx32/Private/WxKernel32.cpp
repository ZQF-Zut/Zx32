#include <Zx32/Private/WxKernel32.h>


namespace Zx32::Kernel32
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

    auto CreateFileU8(const std::string_view u8FilePath, CreateFileU8_Property Properties) -> std::optional<SysID>
    {
        const auto hfile = ::CreateFileW(
            Utils::ApiStrCvt(u8FilePath).first.data(),
            static_cast<DWORD>(Properties.dwDesiredAccess),
            static_cast<DWORD>(Properties.dwShareMode),
            Properties.lpSecurityAttributes,
            static_cast<DWORD>(Properties.dwCreationDisposition),
            static_cast<DWORD>(Properties.dwFlagsAndAttributes),
            Properties.hTemplateFile);
        return (hfile != INVALID_HANDLE_VALUE) ? std::optional<SysID>{ hfile } : std::nullopt;
    }

    auto WriteFile(SysID hFile, std::span<const uint8_t> spData, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>
    {
        DWORD written{};
        bool status = (::WriteFile(hFile, spData.data(), static_cast<DWORD>(spData.size_bytes()), &written, lpOverlapped) != FALSE);
        return status ? std::optional<size_t>{ static_cast<size_t>(written) } : std::nullopt;
    }

    auto ReadFile(SysID hFile, std::span<uint8_t> spBuffer, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>
    {
        DWORD read{};
        bool status = (::ReadFile(hFile, spBuffer.data(), static_cast<DWORD>(spBuffer.size_bytes()), &read, lpOverlapped) != FALSE);
        return status ? std::optional<size_t>{ static_cast<size_t>(read) } : std::nullopt;
    }

    auto GetFileSizeEx(SysID hFile) -> std::optional<uint64_t>
    {
        LARGE_INTEGER file_size{};
        ::GetFileSizeEx(hFile, &file_size);
        return static_cast<uint64_t>(file_size.QuadPart);
    }

    auto SetFilePointerEx(SysID hFile, uint64_t nOffset, MoveWayEnum eMoveWay) -> std::optional<uint64_t>
    {
        LARGE_INTEGER new_file_pointer;
        LARGE_INTEGER move_distance = { .QuadPart = static_cast<LONGLONG>((nOffset)) };
        bool status = (::SetFilePointerEx(hFile, move_distance, &new_file_pointer, static_cast<DWORD>(eMoveWay)) != FALSE);
        return status ? std::optional<uint64_t>{ static_cast<uint64_t>(new_file_pointer.QuadPart) } : std::nullopt;
    }

    auto CloseHandle(SysID hFile) -> bool
    {
        return ::CloseHandle(hFile) != FALSE;
    }

    auto FlushFileBuffers(SysID hFile) -> bool
    {
        return ::FlushFileBuffers(hFile) != FALSE;
    }

    auto CreateDirectoryU8(const std::string_view u8DirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes) -> bool
    {
        return ::CreateDirectoryW(Utils::ApiStrCvt(u8DirName).first.data(), lpSecurityAttributes) != FALSE;
    }

    auto RemoveDirectoryU8(const std::string_view u8DirName) -> bool
    {
        return ::RemoveDirectoryW(Utils::ApiStrCvt(u8DirName).first.data()) != FALSE;
    }

    auto GetFileAttributesU8(const std::string_view u8Path) -> size_t
    {
        return ::GetFileAttributesW(Utils::ApiStrCvt(u8Path).first.data());
    }

    auto DeleteFileU8(const std::string_view u8FilePath) -> bool
    {
        return ::DeleteFileW(Utils::ApiStrCvt(u8FilePath).first.data()) != FALSE;
    }

    auto CopyFileU8(const std::string_view u8ExistFilePath, const std::string_view u8NewFilePath, bool bFailIfExists) -> bool
    {
        return ::CopyFileW(Utils::ApiStrCvt(u8ExistFilePath).first.data(), Utils::ApiStrCvt(u8NewFilePath).first.data(), bFailIfExists ? TRUE : FALSE) != FALSE;
    }

    auto MoveFileU8(const std::string_view u8ExistFilePath, const std::string_view u8NewFilePath) -> bool
    {
        return ::MoveFileW(Utils::ApiStrCvt(u8ExistFilePath).first.data(), Utils::ApiStrCvt(u8NewFilePath).first.data()) != FALSE;
    }

    auto GetStdHandle(STDHandleEnum eHandle) -> std::optional<SysID>
    {
        const auto handle = ::GetStdHandle(static_cast<DWORD>(eHandle));
        return ((handle != INVALID_HANDLE_VALUE) && (handle != 0)) ? std::optional<SysID>{ handle } : std::nullopt;
    }

    auto WriteConsoleU8(SysID hConsoleOutput, const std::string_view u8Text, void* lpReserved) -> std::optional<size_t>
    {
        DWORD written{};
        auto text_wstr = Utils::ApiStrCvt(u8Text);
        bool status = (::WriteConsoleW(hConsoleOutput, text_wstr.first.data(), static_cast<DWORD>(text_wstr.first.size()), &written, lpReserved) != FALSE);
        return status ? std::optional<size_t>{ static_cast<size_t>(written) } : std::nullopt;
    }
} // namespace Wx32::Kernel32
