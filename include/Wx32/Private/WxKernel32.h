#pragma once
#include <Wx32/Utils.h>
#include <optional>
#include <span>

namespace Wx32::Kernel32
{
auto GetModuleFileNameU8(HMODULE hModule) -> MbcsStr_t;
auto GetCurrentDirectoryU8() -> MbcsStr_t;
auto CreateFileU8(const std::string_view& u8FilePath, size_t dwDesiredAccess, size_t dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, size_t dwCreationDisposition, size_t dwFlagsAndAttributes, HANDLE hTemplateFile) -> std::optional<HANDLE>;
auto WriteFile(HANDLE hFile, std::span<uint8_t> spData, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>;
auto ReadFile(HANDLE hFile, std::span<uint8_t> spBuffer, LPOVERLAPPED lpOverlapped) -> std::optional<size_t>;
auto GetFileSizeEx(HANDLE hFile) -> std::optional<uint64_t>;
auto SetFilePointerEx(HANDLE hFile, uint64_t nOffset, size_t dwMoveMethod) -> std::optional<uint64_t>;
auto CloseHandle(HANDLE hFile) -> bool;
auto FlushFileBuffers(HANDLE hFile) -> bool;
auto CreateDirectoryU8(const std::string_view& u8DirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes) -> bool;
auto RemoveDirectoryU8(const std::string_view& u8DirName) -> bool;
auto GetFileAttributesU8(const std::string_view& u8Path) -> size_t;
auto DeleteFileU8(const std::string_view& u8FilePath) -> bool;
auto CopyFileU8(const std::string_view& u8ExistFilePath, const std::string_view& u8NewFilePath, bool bFailIfExists) -> bool;
auto MoveFileU8(const std::string_view& u8ExistFilePath, const std::string_view& u8NewFilePath) -> bool;
auto WriteConsoleU8(HANDLE hConsoleOutput, const std::string_view& u8Text, void* lpReserved) -> std::optional<size_t>;
} // namespace Wx32::Kernel32