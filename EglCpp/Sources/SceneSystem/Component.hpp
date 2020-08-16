#pragma once

#include "../Reflection/Reflectable.hpp"
#include "../Reflection/DynamicType.hpp"

namespace Egliss
{
	class GameObject;
	class Component : public Reflection::Reflectable
	{
	public:
		using GameObjectT = GameObject;
		Component() :
			Reflection::Reflectable(Reflection::StaticTypeDescription<Component>::Id) {}
		void Initialize(GameObjectT* gameObject);

		const Reflection::DynamicTypeDescription& TypeInfo() const
		{
			return Reflection::DynamicTypeManager::TypeOf(this->TypeId);
		}

		GameObjectT& GameObject()
		{
			return *this->_gameObject;
		}
		const GameObjectT& GameObject() const
		{
			return *this->_gameObject;
		}
	private:
		GameObjectT* _gameObject = nullptr;
	};
}