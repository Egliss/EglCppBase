#pragma once

#include "Dialog.hpp"

struct IFileDialog;

namespace Egliss::Dialog
{
	class OpenFolderDialog
	{
	public:
		OpenFolderDialog();
		~OpenFolderDialog();

		FolderDialogOption& Option() { return this->_option; }
		const FolderDialogOption& Option()const { return this->_option; }

		const std::string& ResultDirectory()const { return this->_selectedDirectory; }

		FileDialogResult Open();
		void Release();
	private:
		void CreateDialogInstance();

		std::string _selectedDirectory;
		FolderDialogOption _option;
		IFileDialog* _dialog;
	};
}
