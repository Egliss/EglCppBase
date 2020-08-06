#include "pch.h"
#include "Reflectable.hpp"
#include "DynamicType.hpp"
#include "../Utility/StringUtility.hpp"

using namespace Egliss::Reflection;

void Reflectable::Validate(int typeId)
{
	if (this->TryFastValidate(typeId))
		return;
	if (DynamicTypeManager::Find(typeId) == nullptr)
		throw std::exception(StringUtility::Format("inputed type id({0}) not found", typeId).c_str());
	if (DynamicTypeManager::Find(TypeId) == nullptr)
		throw std::exception(StringUtility::Format("instance saved type id({0}) not found", TypeId).c_str());
	
	auto text = StringUtility::Format("instance saved type id is {0}({1}) but, input type is {2}({3}). please check deriver type's constructor.",
									  DynamicTypeManager::TypeOf(typeId).name,
									  DynamicTypeManager::TypeOf(typeId).id,
									  DynamicTypeManager::TypeOf(TypeId).name,
									  DynamicTypeManager::TypeOf(TypeId).id
	);
	throw std::exception(text.c_str());
}
