#pragma once

#include <string>

namespace Egliss::Dialog
{
	enum class FileDialogResultType
	{
		Error = 0,
		Cancel,
		Success,
	};
	struct FileDialogResult
	{
		FileDialogResult() = default;
		FileDialogResult(FileDialogResultType result) : Result(result) {}

		FileDialogResultType Result;

		operator bool()const { return this->Result == FileDialogResultType::Success; }
	};


	struct FileDialogOption
	{
		std::string defaultDirectory;
		std::string fileExtensionFilter;
		bool isRestart = true;
		bool toSlash = true;
	};
	struct FolderDialogOption
	{
		std::string defaultDirectory;
		bool isRestart = true;
		bool toSlash = true;
	};
}


