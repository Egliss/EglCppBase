#include "pch.h"
#include "StreamReader.hpp"

#include <sstream>

using namespace Egliss;

StreamReader::StreamReader(const std::filesystem::path& path, IOMode type) :
	_type(type),
	_path(path)
{
	this->Open();
}

StreamReader& StreamReader::operator=(StreamReader&& param) noexcept
{
	this->_stream = std::move(param._stream);
	this->_path = std::move(param._path);
	this->_type = param._type;
	return *this;
}

char StreamReader::Read()
{
	char word;
	this->_stream.read(&word, 1);
	return word;
}

void StreamReader::Read(char* buffer, const std::size_t& length)
{
	this->_stream.read(buffer, length);
}

std::string StreamReader::ReadLine()
{
	std::string str;
	std::getline(this->_stream, str);
	return str;
}

std::string StreamReader::ReadToEnd()
{
	std::ostringstream ss;
	ss << this->_stream.rdbuf();
	return ss.str();
}

void StreamReader::ReadToEnd(std::stringstream& buffer)
{
	buffer << this->_stream.rdbuf();
}

std::vector<unsigned char> StreamReader::ReadAllBytes()
{
	this->_stream.seekg(0, std::ios::end);
	const auto length = this->_stream.tellg();
	this->_stream.seekg(0, std::ios::beg);

	std::vector<unsigned char> buffer(length);
	this->_stream.read(reinterpret_cast<char*>(&buffer[0]), length);
	return buffer;
}

bool StreamReader::Open()
{
	this->Close();

	this->_stream = std::ifstream(this->_path.string(), static_cast<int>(this->_type));
	if (!this->_stream.is_open())
	{
		return false;
	}
	return true;
}

void StreamReader::Close()
{
	if (!this->IsReleased())
	{
		this->_stream.close();
	}
}
