#include "pch.h"

#include <stack>
#include <filesystem>

#include "StringUtility.h"

using namespace std;
using namespace Egliss;

std::string StringUtility::Replace(const std::string& text, char from, const std::string& to)
{
	std::string dest = text;
	std::stack<int> insertPoints;
	// from str length is 1
	const auto addLength = to.length() - 1;
	const auto oldLength = dest.length();

	//record insert point
	for (auto L10 = 0U; L10 < dest.length(); L10++)
	{
		if (dest[L10] == from)
			insertPoints.push(L10);
	}

	//insert
	auto backIndex = dest.length();
	dest.resize(dest.length() + insertPoints.size() * addLength);

	while (insertPoints.size())
	{
		auto index = insertPoints.top();
		auto moveIndex = backIndex - (backIndex - index) + insertPoints.size() * addLength;
		auto copyIndex = moveIndex - addLength;
		//move the non replaced str
		char* destPtr = dest.data() + moveIndex;
		const char* sourcePtr = dest.data() + index;
		std::memmove(destPtr, sourcePtr, backIndex - index);

		//copy the replaced str
		destPtr = dest.data() + copyIndex;
		sourcePtr = to.data();
		std::memcpy(destPtr, sourcePtr, to.length());

		//update last
		backIndex = index;
		insertPoints.pop();
	}
	return dest;
}

std::string StringUtility::Replace(const std::string& text, const std::string& from, const std::string& to)
{
	std::string dest = text;
	auto pos = dest.find(from);

	while (pos != std::string::npos)
	{
		dest.replace(pos, from.length(), to);
		pos = dest.find(from, pos + to.length());
	}

	return dest;
}

void StringUtility::Split(const std::string& source, char split, std::vector<std::string>& output)

{
	std::string item;
	for (char ch : source)
	{
		if (ch == split)
		{
			if (!item.empty())
				output.push_back(item);
			item.clear();
		}
		else
		{
			item += ch;
		}
	}
	if (!item.empty())
		output.push_back(item);
}

void StringUtility::Split(const std::string& source, const std::string& split, std::vector<std::string>& result)
{
	result.clear();

	std::string::size_type pos = 0;

	while (pos != std::string::npos)
	{
		auto p = source.find(split, pos);

		if (p == std::string::npos)
		{
			result.push_back(source.substr(pos));
			break;
		}
		else {
			result.push_back(source.substr(pos, p - pos));
		}

		pos = p + split.size();
	}
}

std::wstring StringUtility::ToWString(const char* str)
{
	using namespace std::filesystem;
	auto p = path(str);
	return p.wstring();
}

std::string StringUtility::ToString(const wchar_t* str)
{
	using namespace std::filesystem;
	auto p = path(str);
	return p.string();
}
