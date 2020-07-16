#pragma once

#include "Dialog.hpp"

namespace Egliss::Dialog
{
	class OpenFileDialog
	{
	public:
		OpenFileDialog() = default;
		~OpenFileDialog() = default;

		FileDialogOption& Option() { return this->_option; }
		const FileDialogOption& Option()const { return this->_option; }

		const std::string& SelectedPath()const { return this->_selectedFile; }

		FileDialogResult Open();
	private:
		std::string _selectedFile;
		FileDialogOption _option;
	};
}