#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
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
}

namespace Wx32
{
	template<class T>
	struct ForceTypeWrapper_Basic
	{
		T m_value;

		ForceTypeWrapper_Basic() : m_value{} {}
		explicit ForceTypeWrapper_Basic(T value) : m_value{ value } {}
		operator T() { return m_value; }
	};

	
	using SysID = ForceTypeWrapper_Basic<HANDLE>;
	using WndID = ForceTypeWrapper_Basic<HWND>;
}

namespace Wx32
{
	// https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
	enum class CodePage :size_t
	{
		DEF = static_cast<size_t>(CP_ACP),
		GBK = static_cast<size_t>(936),
		SJIS = static_cast<size_t>(932),
		UTF8 = static_cast<size_t>(CP_UTF8),
		Cyrillic = static_cast<size_t>(1251),
	};

	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-setfilepointer
	enum class MoveWayEnum:size_t
	{
		Beg = static_cast<size_t>(FILE_BEGIN),
		Cur = static_cast<size_t>(FILE_CURRENT),
		End = static_cast<size_t>(FILE_END),
	};

	// https://learn.microsoft.com/en-us/windows/console/getstdhandle
	enum class STDHandleEnum : size_t
	{
		Input = static_cast<size_t>(STD_INPUT_HANDLE),
		Output = static_cast<size_t>(STD_OUTPUT_HANDLE),
		Error = static_cast<size_t>(STD_ERROR_HANDLE),
	};
} // namespace Wx32

namespace Wx32
{
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
	struct CreateFileU8_Property
	{
		size_t dwCreationDisposition;
		size_t dwDesiredAccess;
		size_t dwShareMode;
		size_t dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
		SysID hTemplateFile = SysID{ nullptr };
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr;
	};
}