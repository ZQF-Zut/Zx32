#include <Zx32/APIs.h>
#include <stdexcept>


static auto TestStr()
{
    // mbcs to widestr
    {
        wchar_t buffer[100]{};
        auto wide_str_buffer_sv = Zx32::Utils::StrCvtForce(Zx32::Utils::ForceU8Str(u8"这是一段中文"), buffer, Zx32::CodePage::UTF8);
        Zx32::WideStr_t wide_str_force = Zx32::Utils::StrCvtForce(Zx32::Utils::ForceU8Str(u8"这是一段中文"), Zx32::CodePage::UTF8);
        Zx32::WideStr_t wide_str_safe = Zx32::Utils::StrCvtSafe(Zx32::Utils::ForceU8Str(u8"这是一段中文"), Zx32::CodePage::UTF8);
        if (wide_str_force.first != L"这是一段中文") { return false; }
        if (wide_str_safe.first != L"这是一段中文") { return false; }
        if (wide_str_buffer_sv != L"这是一段中文") { return false; }

        try
        {
            wchar_t buffer_small[2]{};
            auto wide_str_buffer_sm_sv = Zx32::Utils::StrCvtForce(Zx32::Utils::ForceU8Str(u8"这是一段中文"), buffer_small, Zx32::CodePage::UTF8);
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
        Zx32::MbcsStr_t mbc_str0_force = Zx32::Utils::StrCvtForce(L"这是一段中文", Zx32::CodePage::UTF8);
        Zx32::MbcsStr_t mbc_str0_safe = Zx32::Utils::StrCvtSafe(L"这是一段中文", Zx32::CodePage::UTF8);
        if (mbc_str0_force.first != Zx32::Utils::ForceU8Str(u8"这是一段中文")) { return false; }
        if (mbc_str0_safe.first != Zx32::Utils::ForceU8Str(u8"这是一段中文")) { return false; }
    }

    // try error covert
    {
        Zx32::MbcsStr_t mbcs_str = Zx32::Utils::StrCvtSafe(L"这是一段UTF16的中文字符, 但使用SJIS转换, 肯定丢失字符", Zx32::CodePage::SJIS);
        Zx32::WideStr_t wide_str = Zx32::Utils::StrCvtSafe(Zx32::Utils::ForceU8Str(u8"这是一段UTF8的中文字符, 但使用GBK转换, 肯定会出错"), Zx32::CodePage::GBK);
    }

    return true;
}

using namespace Zx32;

auto main() -> int
{
    TestStr();

    API::WriteConsoleU8(API::GetStdHandle(STDHandleEnum::Output).value(), Utils::ForceU8Str(u8"utf8输出测试\n"));

    API::MessageBoxU8(Utils::ForceU8Str(u8"测试UTF8文本1"), Utils::ForceU8Str(u8"这是UTF8文本"), MB_OK);
    API::MessageBoxU8(Utils::ForceU8Str(u8"获取程序当前目录"), API::GetCurrentDirectoryU8().first.data(), MB_OK);
    API::MessageBoxU8(Utils::ForceU8Str(u8"获取程序当前路径"), API::GetModuleFileNameU8().first.data(), MB_OK);

    if (const auto hfile_opt = API::CreateFileU8(Utils::ForceU8Str(u8"1.txt"), { CREATE_ALWAYS ,(GENERIC_WRITE | GENERIC_READ), FILE_SHARE_READ }))
    {
        const auto& hfile = hfile_opt.operator*();

        std::string_view write_str = Utils::ForceU8Str(u8"asjfgauifuiwf,测试写入输入");
        API::WriteFile(hfile, { reinterpret_cast<const uint8_t*>(write_str.data()), write_str.size() });

        API::SetFilePointerEx(hfile, 0, MoveWayEnum::Beg);

        std::array<uint8_t, 33> read_buffer{};
        API::ReadFile(hfile, read_buffer);

        std::string_view read_str = { reinterpret_cast<char*>(read_buffer.data()), read_buffer.size() - 1 };
        if (read_str == write_str)
        {
            API::WriteConsoleU8(API::GetStdHandle(STDHandleEnum::Output).value(), Utils::ForceU8Str(u8"Equal"));
        }
    }
}
