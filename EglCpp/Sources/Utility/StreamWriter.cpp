#include "pch.h"
#include "StreamWriter.hpp"

using namespace Egliss;

StreamWriter::StreamWriter(const std::filesystem::path& path, IOMode type) :
	_path(path),
	_type(type)
{
	this->Open();
}

StreamWriter& StreamWriter::operator=(StreamWriter&& stream)
{
	this->_stream = std::move(stream._stream);
	this->_path = std::move(stream._path);
	this->_type = stream._type;
	return *this;
}

bool StreamWriter::Open()
{
	this->Close();

	this->_stream = std::ofstream(this->_path.string().data(), static_cast<int>(this->_type));
	if (!this->_stream.is_open())
	{
		return false;
	}
	return true;
}

void StreamWriter::Close()
{
	if (!this->IsReleased())
	{
		this->_stream.close();
	}
}
