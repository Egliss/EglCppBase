#pragma once

#include <vector>
#include <unordered_map>

namespace Egliss
{
	template<class Key, class Element>
	class IndexlizedDictionary
	{
		using ContainerT = IndexlizedDictionary<Key, Element>;
	public:
		IndexlizedDictionary() = default;
		IndexlizedDictionary(const ContainerT& container)
		{
			*this = container;
		}
		IndexlizedDictionary(ContainerT&& container)
		{
			*this = std::move(container);
		}
		ContainerT& operator=(const ContainerT& container)
		{
			this->_items = container._items;
			this->_indexMap = container._indexMap;
		}
		ContainerT& operator=(ContainerT&& container)
		{
			this->_items = std::move(container._items);
			this->_indexMap = std::move(container._indexMap);
		}

		int Add(const Key& key, const Element& item)
		{
			this->_items.push_back(item);
			const int index = (int)this->_items.size() - 1;
			this->_indexMap.emplace(key, index);

			return index;
		}
		int Add(const Key& key, Element&& item)
		{
			this->_items.push_back(std::move(item));
			const int index = (int)this->_items.size() - 1;
			this->_indexMap.emplace(std::make_pair(key, index));

			return index;
		}
		int Add(Key&& key, const Element& item)
		{
			this->_items.push_back(item);
			const int index = (int)this->_items.size() - 1;
			this->_indexMap.emplace(std::move(key), index);

			return index;
		}
		int Add(Key&& key, Element&& item)
		{
			this->_items.push_back(std::move(item));
			const int index = (int)this->_items.size() - 1;
			this->_indexMap.emplace(std::move(key), index);

			return index;
		}

		bool Exist(const Key& key)const
		{
			return this->_indexMap.count(key) == 1;
		}
		int GetIndex(const Key& key)const
		{
			return this->_indexMap.at(key);
		}
		int GetIndex(const Key& key)
		{
			return this->_indexMap.at(key);
		}

		Element& GetAt(int index)
		{
			return this->_items.at(index);
		}
		Element& GetAt(const Key& key)
		{
			const int index = this->GetIndex(key);
			return this->GetAt(index);
		}
		const Element& GetAt(int index)const
		{
			return this->_items.at(index);
		}
		const Element& GetAt(const Key& key)const
		{
			const int index = this->GetIndex(key);
			return this->GetAt(index);
		}
		const std::vector<Element>& GetAll()const
		{
			return this->_items;
		}
		const std::unordered_map<Key, int>& GetIndexMap()const
		{
			return this->_indexMap;
		}

	private:

		std::vector<Element> _items;
		std::unordered_map<Key, int> _indexMap;
	};
}