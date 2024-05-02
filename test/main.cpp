#include <Wx32/APIs.h>

auto main() -> int
{
    Wx32::API::MessageBoxU8(nullptr, "测试UTF8文本1", "这是UTF8文本", MB_OK);

    Wx32::API::MessageBoxU8(nullptr, "获取程序当前目录", Wx32::API::GetCurrentDirectoryU8().first.data(), MB_OK);
    Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), "\n", nullptr);

    Wx32::API::MessageBoxU8(nullptr, "获取程序当前路径", Wx32::API::GetModuleFileNameU8(nullptr).first.data(), MB_OK);
    Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), "\n", nullptr);

    Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), "utf8输出测试\n", nullptr);

    HANDLE hfile = Wx32::API::CreateFileU8("1.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr).value();
    std::string_view write_str = "asjfgauifuiwf,测试写入输入";
    Wx32::API::WriteFile(hfile, { reinterpret_cast<const uint8_t*>(write_str.data()), write_str.size() }, nullptr);
    std::array<uint8_t, 33> buffer{};
    Wx32::API::SetFilePointerEx(hfile, 0, FILE_BEGIN);
    Wx32::API::ReadFile(hfile, buffer, nullptr);

    std::string_view read_str = { reinterpret_cast<char*>(buffer.data()), buffer.size() - 1 };
    if (read_str == write_str)
    {
        Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), "Equal\n", nullptr);
    }
}