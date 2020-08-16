#pragma once

#include <string>
#include <vector>

#include "../Reflection/StaticType.hpp"
#include "../Reflection/DynamicType.hpp"

namespace Egliss
{
	class Component;
	class GameObject
	{
	public:
		void Update();

		template<class T>
		static T& GetComponent()
		{
			// GetComponent failed. the type is ambiguous definition
			// please include T definition header.
			constexpr auto id = StaticType<T>::Id;
		#ifdef _DEBUG
			constexpr auto name = StaticType<T>::Name;
		#endif // _DEBUG
			auto& ref = *(_components[id].get());
			return static_cast<T&>(ref);
		}
		template<class T>
		static T* FindComponent()
		{
			constexpr auto id = StaticType<T>::Id;
			if (Reflection::DynamicTypeManager::Find(id) == nullptr)
				return nullptr;

			auto& ref = *(_components[id].get());
			return static_cast<T&>(ref);
		}
		int Id() const
		{
			return this->_id;
		}
		const std::string& Name() const
		{
			return this->_name;
		}
	private:
		int _id = -1;
		std::string _name = "";
		std::vector<Component*> _components;
	};
}
