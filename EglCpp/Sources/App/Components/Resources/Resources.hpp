#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../../IApplicationComponent.hpp"
#include "Resource.hpp"

namespace Egliss
{
	class Resources : public IApplicationComponent
	{
		friend class Resource;
	public:
		const std::vector<std::shared_ptr<Resource>>& GetDynamicReosurces() const
		{
			return this->_dynamicResource;
		}
		std::shared_ptr<Resource> FindPhysicalResource(const std::string& physicalPath)const
		{
			const auto itr = _physicalResources.find(physicalPath);
			if (itr == this->_physicalResources.end())
				return nullptr;

			return itr->second;
		}


		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual std::string Name() const override { return "Resources"; }

	private:
		// physical directory referenced resource
		std::unordered_map <std::string, std::shared_ptr<Resource>> _physicalResources;
		std::vector<std::shared_ptr<Resource>> _dynamicResource;

		void Register(std::shared_ptr<Resource> resource);
	};
}