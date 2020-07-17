#pragma once

#include <string>

namespace Egliss
{
	class Resource
	{
	public:
		virtual ~Resource() = default;

		const std::string& Key() const
		{
			return this->_key;
		}

		void Initialize(const std::string& key, const std::string& path);
	protected:
		std::string _key;

		virtual bool InternalInitialize(const std::string& path) = 0;
	};
}