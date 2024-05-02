#pragma once
#include <Windows.h>
#include <array>
#include <cassert>
#include <memory>
#include <string_view>


namespace Wx32
{
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
auto StrCvtU8ToU16(const std::string_view& u8Str) -> WideStr_t;
auto StrCvtU16ToU8(const std::wstring_view& u16Str) -> MbcsStr_t;

auto PathStrCvtU8ToU16(const std::string_view& u8Str, U16PathBuffer& sfU16Buffer) -> std::wstring_view;
auto PathStrCvtU16ToU8(const std::wstring_view& u16Str, U8PathBuffer& sfU8Buffer) -> std::string_view;
} // namespace Wx32::Utils
