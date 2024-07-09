#pragma once
#include <Zx32/Define.h>
#include <span>


namespace Zx32::Utils
{
    // cvt multibyte-string to wide-string without warning log
    auto StrCvtForce(const std::string_view msStr, const CodePage eCodePage) -> WideStr_t;

    // cvt wide-string to multibyte-string without warning log
    auto StrCvtForce(const std::wstring_view wsStr, const CodePage eCodePage) -> MbcsStr_t;

    // cvt multibyte-string to wide-string use a existing buffer, without warning log
    auto StrCvtForce(const std::string_view msStr, std::span<wchar_t> spBuffer, const CodePage eCodePage) -> std::wstring_view;

    // cvt wide-string to multibyte-string use a existing buffer, without warning log
    auto StrCvtForce(const std::wstring_view wsStr, std::span<char> spBuffer, const CodePage eCodePage) -> std::string_view;

    // cvt multibyte-string to wide-string with warning log
    auto StrCvtSafe(const std::string_view msStr, const CodePage eCodePage) -> WideStr_t;

    // cvt wide-string to multibyte-string with warning log
    auto StrCvtSafe(const std::wstring_view wsStr, const CodePage eCodePage) -> MbcsStr_t;

    // cvt utf8-string to utf16-string with allocting memory (heap memory), without warning log
    auto ApiStrCvt(const std::string_view u8Str) -> WideStr_t;

    // cvt utf16-string to utf8-string with allocting memory (heap memory), without warning log
    auto ApiStrCvt(const std::wstring_view u16Str) -> MbcsStr_t;

    // cvt utf8-string to utf16-string use a existing buffer, without warning log
    auto ApiStrCvt(const std::string_view u8Str, std::span<wchar_t> spBuffer) -> std::wstring_view;

    // cvt utf16-string to utf8-string use a existing buffer, without warning log
    auto ApiStrCvt(const std::wstring_view u16Str, std::span<char> spBuffer) -> std::string_view;

    // u8string to string
    auto ForceU8Str(const std::u8string_view msStr) -> std::string_view;
} // namespace Wx32::Utils
