#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "StaticType.hpp"

namespace Egliss::Reflection
{
	class DynamicTypeDescription
	{
	public:
		DynamicTypeDescription() = default;
		DynamicTypeDescription(int typeId, std::string&& name, bool isAbstract, std::vector<int>&& parentTypeIds, std::function<void* ()>&& constructor):
			id(typeId),
			name(std::move(name)),
			isAbstract(isAbstract),
			parentTypeIds(std::move(parentTypeIds)),
			constructor(std::move(constructor))
		{}

		bool IsChildOf(int typeId) const
		{
			return std::binary_search(this->parentTypeIds.begin(), this->parentTypeIds.end(), typeId);
		}

		const int id;
		const std::string name;
		const bool isAbstract;
		const std::vector<int> parentTypeIds;
		const std::function<void* ()> constructor;
	};
	class DynamicTypeManager
	{
	public:
		static void Initialize();
		static void Finalize()
		{
			_indexedTypes.clear();
			_typesindices.clear();
		}

		static const auto& Types()
		{
			return _indexedTypes;
		}
		static const DynamicTypeDescription& TypeOf(int typeId)
		{
			return _indexedTypes[typeId];
		}
		static const DynamicTypeDescription& TypeOf(const std::string& typeName)
		{
			auto index = _typesindices.at(typeName);
			return TypeOf(index);
		}
		static const DynamicTypeDescription* Find(int typeId)
		{
			if (typeId < 0 || typeId > static_cast<int>(_indexedTypes.size()))
				return nullptr;

			return &_indexedTypes[typeId];
		}
		static const DynamicTypeDescription* Find(const std::string& typeName)
		{
			const auto index = _typesindices.find(typeName);
			if (index == _typesindices.end())
				return nullptr;
			return &_indexedTypes[index->second];
		}

		template<class T>
		static T* XCast(Reflectable* ptr)
		{
			// TODO ��֎�i��reinterpret�Ŕh���ɖ߂���T�֖߂��Ă���reinterpret��IReflectable�ɂ��郉���_��p��
			// ����ŃA�h���X���Y�����܂܂�IReflectable�ɖ߂�̂ł����T�֖߂�
			// ���������̎����Ői�߂�ꍇ�͑��x�������ɑ���Ȃ�
			return dynamic_cast<T*>(ptr);
		}

	private:
		static std::vector<DynamicTypeDescription> _indexedTypes;
		static std::unordered_map<std::string, int> _typesindices;
	};
}