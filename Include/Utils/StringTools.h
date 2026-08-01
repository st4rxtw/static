/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace Utils
{
	class StringTools
	{
	public:
		static std::string ToLower(std::string_view str)
		{
			// to lowercase
			std::string result(str);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return result;
		}

		static std::string ToUpper(std::string_view str)
		{
			// to uppercase
			std::string result(str);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
			return result;
		}

		static std::string Trim(std::string_view str)
		{
			// trim text
			size_t begin = str.find_first_not_of(" \t\n\r\f\v");
			if (begin == std::string_view::npos)
			{
				return std::string();
			}

			size_t end = str.find_last_not_of(" \t\n\r\f\v");
			return std::string(str.substr(begin, end - begin + 1));
		}

		static bool StartsWith(std::string_view str, std::string_view prefix)
		{
			// if it starts with something
			return str.starts_with(prefix);
		}

		static bool EndsWith(std::string_view str, std::string_view suffix)
		{
			// if it ends with something
			return str.ends_with(suffix);
		}

		static std::vector<std::string> Split(std::string_view str, char delimiter)
		{
			// split text
			std::vector<std::string> result;

			size_t start = 0;
			while (start <= str.size())
			{
				size_t pos = str.find(delimiter, start);
				if (pos == std::string_view::npos)
				{
					result.emplace_back(str.substr(start));
					break;
				}

				result.emplace_back(str.substr(start, pos - start));
				start = pos + 1;
			}

			return result;
		}

		static std::wstring S2ws(std::string_view str)
		{
			return std::wstring(str.begin(), str.end());
		}

		static std::string Ws2s(std::wstring_view str)
		{
			return std::string(str.begin(), str.end());
		}
	};
}

