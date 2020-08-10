#pragma once

#include <memory>
#include <string>

namespace Egliss
{
	class Resource
	{
	public:
		Resource();
		virtual ~Resource() = default;

		const std::string& Key() const
		{
			return this->_key;
		}

		void Register(std::shared_ptr<Resource> resource);

		template<class T>
		std::shared_ptr<T> AsRef() const
		{
			if (this->_ownRef.expired())
				return nullptr;

			return this->_ownRef.lock();
		}
	protected:
		std::weak_ptr<Resource> _ownRef;
		int _internalIndex;
		std::string _key;
	};
}