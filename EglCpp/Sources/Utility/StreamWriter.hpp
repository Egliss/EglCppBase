#pragma once

#include "Stream.hpp"

namespace Egliss
{
	class StreamWriter
	{
	public:
		StreamWriter() = default;
		StreamWriter(const std::filesystem::path& path, IOMode type = IOMode::Append);
		StreamWriter(const StreamWriter&) = delete;
		StreamWriter(StreamWriter&& stream)
		{
			*this = static_cast<StreamWriter&&>(stream);
		}

		StreamWriter& operator=(const StreamWriter&) = delete;
		StreamWriter& operator=(StreamWriter&& stream);

		~StreamWriter()
		{
			this->Close();
		}

		void WriteLine(const std::string& str)
		{
			this->_stream.write(str.data(), str.size());
		}
		void Write(const std::string& str)
		{
			this->_stream.write(str.data(), str.length());
		}
		void Write(char str)
		{
			this->_stream.write(&str, 1);
		}
		void Write(const char* buffer, std::size_t length)
		{
			this->_stream.write(buffer, length);
		}
		template<class T>
		void BinaryWrite(const T& val)
		{
			this->_stream.write((const char*)(&val), sizeof(T));
		}
		bool Open();
		void Close();

		const std::filesystem::path& GetPath()const
		{
			return this->_path;
		}
		IOMode WriteMode()const
		{
			return this->_type;
		}
		bool IsFail()const
		{
			return this->_stream.fail();
		}

		void Path(const std::filesystem::path& path)
		{
			this->_path = path;
		}
		void WriteMode(IOMode type)
		{
			this->_type = type;
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

		IOMode _type;
		std::ofstream _stream;
		std::filesystem::path _path;
	};
}