#pragma once
#include <Wx32/Utils.h>
#include <optional>
#include <span>

namespace Wx32::Kernel32
{
auto GetModuleFileNameU8(HMODULE hModule = nullptr) -> MbcsStr_t;
auto GetCurrentDirectoryU8() -> MbcsStr_t;
auto CreateFileU8(const std::string_view u8FilePath, CreateFileU8_Property Property) -> std::optional<SysID>;
auto WriteFile(SysID hFile, std::span<const uint8_t> spData, LPOVERLAPPED lpOverlapped = nullptr) -> std::optional<size_t>;
auto ReadFile(SysID hFile, std::span<uint8_t> spBuffer, LPOVERLAPPED lpOverlapped = nullptr) -> std::optional<size_t>;
auto GetFileSizeEx(SysID hFile) -> std::optional<uint64_t>;
auto SetFilePointerEx(SysID hFile, uint64_t nOffset, MoveWayEnum eMoveWay) -> std::optional<uint64_t>;
auto CloseHandle(SysID hFile) -> bool;
auto FlushFileBuffers(SysID hFile) -> bool;
auto CreateDirectoryU8(const std::string_view u8DirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr) -> bool;
auto RemoveDirectoryU8(const std::string_view u8DirName) -> bool;
auto GetFileAttributesU8(const std::string_view u8Path) -> size_t;
auto DeleteFileU8(const std::string_view u8FilePath) -> bool;
auto CopyFileU8(const std::string_view u8ExistFilePath, const std::string_view u8NewFilePath, bool bFailIfExists) -> bool;
auto MoveFileU8(const std::string_view u8ExistFilePath, const std::string_view u8NewFilePath) -> bool;
auto GetStdHandle(STDHandleEnum eHandle) -> std::optional<SysID>;
auto WriteConsoleU8(SysID hConsoleOutput, const std::string_view u8Text, void* lpReserved = nullptr) -> std::optional<size_t>;
} // namespace Wx32::Kernel32