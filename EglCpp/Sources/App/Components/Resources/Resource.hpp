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
		virtual bool Initialize();
		virtual void Finalize();

		template<class T>
		std::shared_ptr<T> AsRef() const
		{
			if (this->_ownRef.expired())
				return nullptr;

			return this->_ownRef.lock();
		}
		bool Initialized() const
		{
			return this->_initialized;
		}
	protected:
		std::string _key = "";
	private:
		std::weak_ptr<Resource> _ownRef;
		int _internalIndex = -1;
		bool _initialized = false;
	};
}