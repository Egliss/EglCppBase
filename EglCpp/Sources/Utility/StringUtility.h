#pragma once

#include <string>
#include <string_view>
#include <fmt/format.h>

namespace Egliss
{
	class StringUtility
	{
	public:
		static int Compare(const char* s1, const char* s2)
		{
			return std::strcmp(s1, s2);
		}
		static int Compare(std::string_view s1, const char* s2)
		{
			return Compare(s1.data(), s2);
		}
		static int Compare(const char* s1, std::string_view s2)
		{
			return Compare(s1, s2.data());
		}
		static int Compare(std::string_view s1, std::string_view s2)
		{
			return Compare(s1.data(), s2.data());
		}
		static int Compare(const wchar_t* s1, const wchar_t* s2)
		{
			return std::wcscmp(s1, s2);
		}
		static int Compare(std::wstring_view s1, const wchar_t* s2)
		{
			return Compare(s1.data(), s2);
		}
		static int Compare(const wchar_t* s1, std::wstring_view s2)
		{
			return Compare(s1, s2.data());
		}
		static int Compare(std::wstring_view s1, std::wstring_view s2)
		{
			return Compare(s1.data(), s2.data());
		}

		static bool Is(const char* s1, const char* s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(std::string_view s1, const char* s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(const char* s1, std::string_view s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(std::string_view s1, std::string_view s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(const wchar_t* s1, const wchar_t* s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(std::wstring_view s1, const wchar_t* s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(const wchar_t* s1, std::wstring_view s2)
		{
			return Compare(s1, s2) == 0;
		}
		static bool Is(std::wstring_view s1, std::wstring_view s2)
		{
			return Compare(s1, s2) == 0;
		}

		static int ICaseCompare(const char* s1, const char* s2)
		{
			return _stricmp(s1, s2);
		}
		static int ICaseCompare(std::string_view s1, const char* s2)
		{
			return ICaseCompare(s1.data(), s2);
		}
		static int ICaseCompare(const char* s1, std::string_view s2)
		{
			return ICaseCompare(s1, s2.data());
		}
		static int ICaseCompare(std::string_view s1, std::string_view s2)
		{
			return ICaseCompare(s1.data(), s2.data());
		}
		static int ICaseCompare(const wchar_t* s1, const wchar_t* s2)
		{
			return  _wcsicmp(s1, s2);
		}
		static int ICaseCompare(std::wstring_view s1, const wchar_t* s2)
		{
			return ICaseCompare(s1.data(), s2);
		}
		static int ICaseCompare(const wchar_t* s1, std::wstring_view s2)
		{
			return ICaseCompare(s1, s2.data());
		}
		static int ICaseCompare(std::wstring_view s1, std::wstring_view s2)
		{
			return ICaseCompare(s1.data(), s2.data());
		}

		static bool ICaseIs(const char* s1, const char* s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(std::string_view s1, const char* s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(const char* s1, std::string_view s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(std::string_view s1, std::string_view s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(const wchar_t* s1, const wchar_t* s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(std::wstring_view s1, const wchar_t* s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(const wchar_t* s1, std::wstring_view s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}
		static bool ICaseIs(std::wstring_view s1, std::wstring_view s2)
		{
			return ICaseCompare(s1, s2) == 0;
		}

		static std::string& Replace(std::string& dest, char from, char to)
		{
			for (auto L10 = 0U; L10 < dest.length(); L10++)
			{
				if (dest[L10] == from)
					dest[L10] = to;
			}
			return dest;
		}
		static std::string& Replace(std::string& dest, char from, const std::string& to);
		static std::string& Replace(std::string& dest, const std::string& from, const std::string& to);

		static std::string Repeat(char word, int count)
		{
			return std::string(count, word);
		}

		static void Split(const std::string& source, char split, std::vector<std::string>& output);
		static void Split(const std::string& source, const std::string& split, std::vector<std::string>& result);

		static std::wstring ToWString(const char* str);
		static std::wstring ToWString(const std::string& str)
		{
			return ToWString(str.data());
		}
		static std::string ToString(const wchar_t* str);
		static std::string ToString(const std::wstring& str)
		{
			return ToString(str);
		}

		template<class ...Args>
		static std::string Format(const char* formatText, Args&& ... args)
		{
			return fmt::format(formatText, std::forward<Args&&>(args)...);
		}
		template<class ...Args>
		static std::string Format(const std::string& formatText, Args&& ... args)
		{
			return fmt::format(formatText, std::forward<Args&&>(args)...);
		}
		template<class ...Args>
		static std::string Format(std::string_view formatText, Args&& ... args)
		{
			return fmt::format(formatText, std::forward<Args&&>(args)...);
		}

	private:

		StringUtility() = delete;
		~StringUtility() = delete;
	};
}