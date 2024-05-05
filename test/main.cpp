#include <Wx32/APIs.h>
#include <stdexcept>

static auto TestStr()
{
    // mbcs to widestr
    {
        wchar_t buffer[100]{};
        auto wide_str_buffer_sv = Wx32::Utils::StrCvtForce(Wx32::Utils::ForceU8Str(u8"这是一段中文"), buffer, Wx32::CodePage::UTF8);
        Wx32::WideStr_t wide_str_force = Wx32::Utils::StrCvtForce(Wx32::Utils::ForceU8Str(u8"这是一段中文"), Wx32::CodePage::UTF8);
        Wx32::WideStr_t wide_str_safe = Wx32::Utils::StrCvtSafe(Wx32::Utils::ForceU8Str(u8"这是一段中文"), Wx32::CodePage::UTF8);
        if (wide_str_force.first != L"这是一段中文") { return false; }
        if (wide_str_safe.first != L"这是一段中文") { return false; }
        if (wide_str_buffer_sv != L"这是一段中文") { return false; }

        try
        {
            wchar_t buffer_small[2]{};
            auto wide_str_buffer_sm_sv = Wx32::Utils::StrCvtForce(Wx32::Utils::ForceU8Str(u8"这是一段中文"), buffer_small, Wx32::CodePage::UTF8);
        }
        catch (const std::runtime_error& err)
        {
            if (::strcmp(err.what(), "ApiStrCvt: buffer too small"))
            {
                return false;
            }
        }
    }

    // widestr to mbcs
    {
        Wx32::MbcsStr_t mbc_str0_force = Wx32::Utils::StrCvtForce(L"这是一段中文", Wx32::CodePage::UTF8);
        Wx32::MbcsStr_t mbc_str0_safe = Wx32::Utils::StrCvtSafe(L"这是一段中文", Wx32::CodePage::UTF8);
        if (mbc_str0_force.first != Wx32::Utils::ForceU8Str(u8"这是一段中文")) { return false; }
        if (mbc_str0_safe.first != Wx32::Utils::ForceU8Str(u8"这是一段中文")) { return false; }
    }

    // try error covert
    {
        Wx32::MbcsStr_t mbcs_str = Wx32::Utils::StrCvtSafe(L"这是一段UTF16的中文字符, 但使用SJIS转换, 肯定丢失字符", Wx32::CodePage::SJIS);
        Wx32::WideStr_t wide_str = Wx32::Utils::StrCvtSafe(Wx32::Utils::ForceU8Str(u8"这是一段UTF8的中文字符, 但使用GBK转换, 肯定会出错"), Wx32::CodePage::GBK);
    }

    return true;
}

auto main() -> int
{
    TestStr();

    Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), Wx32::Utils::ForceU8Str(u8"utf8输出测试\n"), nullptr);

    Wx32::API::MessageBoxU8(nullptr, Wx32::Utils::ForceU8Str(u8"测试UTF8文本1"), Wx32::Utils::ForceU8Str(u8"这是UTF8文本"), MB_OK);
    Wx32::API::MessageBoxU8(nullptr, Wx32::Utils::ForceU8Str(u8"获取程序当前目录"), Wx32::API::GetCurrentDirectoryU8().first.data(), MB_OK);
    Wx32::API::MessageBoxU8(nullptr, Wx32::Utils::ForceU8Str(u8"获取程序当前路径"), Wx32::API::GetModuleFileNameU8(nullptr).first.data(), MB_OK);

    HANDLE hfile = Wx32::API::CreateFileU8(Wx32::Utils::ForceU8Str(u8"1.txt"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr).value();
    std::string_view write_str = Wx32::Utils::ForceU8Str(u8"asjfgauifuiwf,测试写入输入");
    Wx32::API::WriteFile(hfile, { reinterpret_cast<const uint8_t*>(write_str.data()), write_str.size() }, nullptr);
    std::array<uint8_t, 33> buffer{};
    Wx32::API::SetFilePointerEx(hfile, 0, FILE_BEGIN);
    Wx32::API::ReadFile(hfile, buffer, nullptr);

    std::string_view read_str = { reinterpret_cast<char*>(buffer.data()), buffer.size() - 1 };
    if (read_str == write_str)
    {
        Wx32::API::WriteConsoleU8(::GetStdHandle(STD_OUTPUT_HANDLE), Wx32::Utils::ForceU8Str(u8"Equal"), nullptr);
    }
}