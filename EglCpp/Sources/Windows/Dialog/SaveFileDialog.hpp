#pragma once
#include "Dialog.hpp"

namespace Egliss::Dialog
{
	class SaveFileDialog
	{
	public:
		SaveFileDialog() = default;
		~SaveFileDialog() = default;

		FileDialogOption& Option() { return this->_option; }
		const FileDialogOption& Option()const { return this->_option; }

		const std::string& SelectedFile()const { return this->_selectedFile; }

		FileDialogResult Open();
	private:
		std::string _selectedFile;
		FileDialogOption _option;
	};
}