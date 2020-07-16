#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../IApplicationComponent.hpp"

namespace Egliss
{
	class Resource
	{
	public:
		const std::string& Key()const { return this->_key; }
		const std::string& Path()const { return this->_path; }

	protected:
		std::string _key;
		std::string _path;
	};

	class Resources : public IApplicationComponent
	{
	public:
		std::shared_ptr<Resource> FindResource(const std::string& key) const
		{
			throw std::exception("not implemented.");
		}
		std::shared_ptr<Resource> FindPhysicalResource(const std::string& physicalPath)const
		{
			const auto itr = _physicalResources.find(physicalPath);
			if (itr == this->_physicalResources.end())
				return nullptr;

			return itr->second;
		}

		// physical directory referenced resource
		std::unordered_map <std::string, std::shared_ptr<Resource>> _physicalResources;

		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual std::string Name() const override { return "Resources"; }
	};
}