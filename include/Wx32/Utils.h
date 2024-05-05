#pragma once
#include <Windows.h>
#include <array>
#include <cassert>
#include <memory>
#include <span>
#include <string_view>

namespace Wx32
{
enum class CodePage
{
    DEF = 0,
    GBK = 936,
    SJIS = 932,
    UTF8 = 65001
};

using MbcsStr_t = std::pair<std::string_view, std::unique_ptr<char[]>>;
using WideStr_t = std::pair<std::wstring_view, std::unique_ptr<wchar_t[]>>;

template <class Char_t>
class PathBufferBased
{
  private:
    bool m_isEnable = true;
    std::array<Char_t, MAX_PATH> m_aBuffer;

  public:
    auto GetDataUnsafe() -> Char_t*
    {
        return m_aBuffer.data();
    }

    auto GetSize() -> size_t
    {
        return m_aBuffer.size();
    }

    auto AccessOnce() -> std::array<Char_t, MAX_PATH>&
    {
        assert(m_isEnable); // "SafePathBuffer: double access"
        m_isEnable = false;
        return m_aBuffer;
    }
};

using U8PathBuffer = PathBufferBased<char>;
using U16PathBuffer = PathBufferBased<wchar_t>;
} // namespace Wx32

namespace Wx32::Utils
{
auto StrCvtForce(const std::string_view& u8Str, const CodePage eCodePage) -> WideStr_t;
auto StrCvtForce(const std::wstring_view& u16Str, const CodePage eCodePage) -> MbcsStr_t;
auto StrCvtForce(const std::string_view& u8Str, std::span<wchar_t> spBuffer, const CodePage eCodePage) -> std::wstring_view;
auto StrCvtForce(const std::wstring_view& u16Str, std::span<char> spBuffer, const CodePage eCodePage) -> std::string_view;

// cvt wide-string to multibyte-string with warning log
auto StrCvt(const std::wstring_view& wsStr, const CodePage eCodePage) noexcept -> MbcsStr_t;

// cvt multibyte-string to wide-string with warning log
auto StrCvt(const std::string_view& msStr, const CodePage eCodePage) noexcept -> WideStr_t;

auto ApiStrCvt(const std::string_view& u8Str) -> WideStr_t;
auto ApiStrCvt(const std::wstring_view& u16Str) -> MbcsStr_t;

// u8string to string
auto ForceU8Str(const std::u8string_view& msStr) -> std::string_view;
} // namespace Wx32::Utils
