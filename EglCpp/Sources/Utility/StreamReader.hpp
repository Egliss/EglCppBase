#pragma once

#include "Stream.hpp"

namespace Egliss
{
	class StreamReader
	{
	public:
		StreamReader() = default;
		StreamReader(const std::filesystem::path& path, IOMode type);
		StreamReader(const StreamReader&) = delete;
		StreamReader(StreamReader&& stream) noexcept
		{
			*this = static_cast<StreamReader&&>(stream);
		}

		StreamReader& operator=(const StreamReader&) = delete;
		StreamReader& operator=(StreamReader&& param) noexcept;

		~StreamReader()
		{
			this->Close();
		}

		char Read();
		void Read(char* buffer, const std::size_t& length);

		std::string ReadLine();
		std::string ReadToEnd();
		void ReadToEnd(std::stringstream& buffer);
		std::vector<unsigned char>ReadAllBytes();

		template<class T>
		void BinaryRead(T& val)
		{
			this->_stream.read((char*)(&val), sizeof(T));
		}
		bool Open();
		void Close();

		const std::filesystem::path& Path()const
		{
			return _path;
		}
		IOMode ReadMode()const
		{
			return _type;
		}

		void Path(const std::filesystem::path& path)
		{
			this->_path = path;
		}
		void ReadMode(IOMode type)
		{
			this->_type = type;
		}

		bool IsEOF()const
		{
			return _stream.eof();
		}
		bool IsFail()const
		{
			return _stream.fail();
		}

		void Release()
		{
			this->Close();
		};
		bool IsReleased() const
		{
			return !this->_stream.is_open();
		};
	private:
		IOMode _type = IOMode::Input;
		std::ifstream _stream;
		std::filesystem::path _path;
	};
}