/*
 *	Copyright 2026-2027 static - hiro
 *
 *	Use of this source code without explicit permission from owner is strictly prohibited.
 */

#pragma once

#include <filesystem>
#include <string>
#include <string_view>

#include <sys/stat.h>

namespace Utils
{
	class Paths
	{
	public:
		static void SetAssetRoot(std::string_view root)
		{
			GetAssetRootRef() = root;
		}

		static const std::string& GetAssetRoot()
		{
			return GetAssetRootRef();
		}

		static std::string Resolve(std::string_view path)
		{
			std::filesystem::path p(path);
			if (p.is_absolute() || GetAssetRootRef().empty())
			{
				return p.string();
			}

			return (std::filesystem::path(GetAssetRootRef()) / p).string();
		}

		static bool PathExists(std::string_view path)
		{
			struct stat info{};
			return stat(std::string(path).c_str(), &info) == 0;
		}

	private:
		static std::string& GetAssetRootRef()
		{
			static std::string root = "data/";
			return root;
		}
	};
}
