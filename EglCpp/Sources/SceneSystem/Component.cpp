#include "pch.h"
#include "Component.hpp"
#include "GameObject.hpp"
#include "../Utility/StringUtility.hpp"

using namespace Egliss;

void Component::Initialize(GameObjectT* gameObject)
{
#ifdef _DEBUG
	if (this->_gameObject != nullptr)
	{
		throw std::exception(StringUtility::Format("the gameobject already binding. old:{0} new:{1}",
							 this->_gameObject->Name(),
							 gameObject->Name()
		).data());
	}
#endif // _DEBUG
	this->_gameObject = gameObject;
}
