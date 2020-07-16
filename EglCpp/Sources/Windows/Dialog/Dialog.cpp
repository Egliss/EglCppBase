#include "pch.h"

#include <Windows.h>
#include <commdlg.h>
#include <filesystem>
#include <shlobj_core.h>

#include "../../Utility/StringUtility.hpp"
#include "../../Utility/MemoryUtility.hpp"

#include "SaveFileDialog.hpp"
#include "OpenFileDialog.hpp"
#include "OpenFolderDialog.hpp"

using namespace Egliss;
using namespace Egliss::Dialog;

FileDialogResult SaveFileDialog::Open()
{
	std::string buffer;
	std::string filterBuffer = "(*.*|AllFile)";
	buffer.resize(0xFFF);
	filterBuffer.resize(0x400);

	OPENFILENAMEA of;
	MemoryUtility::Fill(of);
	of.hwndOwner = nullptr;
	of.lStructSize = sizeof(OPENFILENAMEA);
	of.lpstrFilter = this->_option.fileExtensionFilter.data();
	of.lpstrCustomFilter = nullptr;
	of.nMaxCustFilter = 0;
	of.lpstrFile = buffer.data();
	of.lpfnHook = nullptr;
	of.lpstrFilter = filterBuffer.data();
	of.nFilterIndex = 0;
	of.nMaxFile = 0xFFF;
	of.Flags = OFN_FILEMUSTEXIST;
	of.lpstrInitialDir = this->_option.defaultDirectory.data();

	if (!::GetSaveFileNameA(&of))
	{
		// canceled
		this->_selectedFile.clear();
		return FileDialogResultType::Cancel;
	}
	buffer.shrink_to_fit();
	this->_selectedFile = std::move(buffer);

	if (this->_option.isRestart)
	{
		using PathT = std::filesystem::path;
		this->_option.defaultDirectory = PathT(of.lpstrFile).remove_filename().string();
	}

	// convert 
	if (this->_option.toSlash)
		StringUtility::Replace(this->_selectedFile, "\\", "/");

	return FileDialogResultType::Success;
}

//-------------------------------------------//
// OpenFolderDialog
//-------------------------------------------//

//せっかくComのインスタンスが取れるのでキャッシュ
OpenFolderDialog::OpenFolderDialog() : _dialog(nullptr)
{
	this->CreateDialogInstance();
}

OpenFolderDialog::~OpenFolderDialog()
{
	this->Release();
}

void OpenFolderDialog::CreateDialogInstance()
{
	if (this->_dialog != nullptr)
		return;
	HRESULT hr = CoCreateInstance(
		__uuidof(FileOpenDialog),
		0,
		CLSCTX::CLSCTX_INPROC_SERVER,
		__uuidof(IFileOpenDialog),
		reinterpret_cast<void**>(&this->_dialog));

	if (FAILED(hr))
		this->_dialog = nullptr;
}

FileDialogResult OpenFolderDialog::Open()
{
	this->CreateDialogInstance();

	if (this->_dialog == nullptr)
		return FileDialogResultType::Error;

	HRESULT hr = S_OK;
	auto dialog = this->_dialog;

	// set folder pick mode
	DWORD option;
	hr = this->_dialog->GetOptions(&option);
	option |= _FILEOPENDIALOGOPTIONS::FOS_PICKFOLDERS;

	if (SUCCEEDED(hr))
		hr = dialog->SetOptions(option);
	if (SUCCEEDED(hr))
		hr = dialog->SetTitle(L"フォルダを開く");

	// show the open file dialog
	if (SUCCEEDED(hr))
	{
		hr = dialog->Show(nullptr);
		if (SUCCEEDED(hr))
		{
			IShellItem* shellResult = nullptr;
			hr = dialog->GetResult(&shellResult);
			if (SUCCEEDED(hr))
			{
				// release display name buffer
				wchar_t* selectedText = nullptr;
				hr = shellResult->GetDisplayName(SIGDN::SIGDN_FILESYSPATH, &selectedText);
				if (SUCCEEDED(hr))
				{
					this->_selectedDirectory = StringUtility::ToString(selectedText);
					CoTaskMemFree(selectedText);
				}
				shellResult->Release();
			}
		}
	}
	// OnError
	if (FAILED(hr))
	{
		if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		{
			// User cancelled the dialog...
			return FileDialogResultType::Cancel;
		}

		//ERROR Process
		{
			return FileDialogResultType::Error;
		}
	}

	// convert 
	if (this->_option.toSlash)
		StringUtility::Replace(this->_selectedDirectory, "\\", "/");

	return FileDialogResultType::Success;
}

void OpenFolderDialog::Release()
{
	if (this->_dialog == nullptr)
		return;
	this->_dialog->Release();
}


//-------------------------------------------//
// OpenFileDialog
//-------------------------------------------//

FileDialogResult OpenFileDialog::Open()
{
	std::string buffer;
	std::string filterBuffer = "(*.*|AllFile)";
	buffer.resize(0xFFF);
	filterBuffer.resize(0xFF);

	OPENFILENAMEA of;
	MemoryUtility::Fill(of);
	of.hwndOwner = nullptr;
	of.lStructSize = sizeof(OPENFILENAMEA);
	of.lpstrFilter = this->_option.fileExtensionFilter.data();
	of.lpstrCustomFilter = nullptr;
	of.nMaxCustFilter = 0;
	of.lpstrFile = buffer.data();
	of.lpfnHook = nullptr;
	of.lpstrFilter = filterBuffer.data();
	of.nFilterIndex = 0;
	of.nMaxFile = 0xFFF;
	of.Flags = OFN_FILEMUSTEXIST;
	of.lpstrInitialDir = this->_option.defaultDirectory.data();

	if (!::GetOpenFileNameA(&of))
	{
		// canceled
		this->_selectedFile.clear();
		return FileDialogResultType::Cancel;
	}
	this->_selectedFile = std::move(buffer);

	if (this->_option.isRestart)
	{
		using PathT = std::filesystem::path;
		this->_option.defaultDirectory = PathT(of.lpstrFile).remove_filename().string();
	}

	// convert 
	if (this->_option.toSlash)
		StringUtility::Replace(this->_selectedFile, "\\", "/");

	return FileDialogResultType::Success;
}
