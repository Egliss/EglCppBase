#pragma once

#include <type_traits>
#include <vector>
#include <functional>

namespace Egliss
{
	/// <summary>
	/// This is invalid event.<para/>
	/// Please input type -> T() or T(ArgT)
	/// </summary>
	template<class T>
	class Event
	{
	};

	template<class T>
	class Event<T()>
	{
		using FunctionT = std::function<T()>;
		template<class U>
		static constexpr bool IsVoid = std::is_same_v<U, void>;
	public:
		void Add(FunctionT&& action)
		{
			this->_functions.push_back(std::move(action));
		}
		void Clear()
		{
			this->_functions.clear();
		}
		int Count()const
		{
			return (int)this->_functions.size();
		}

		template<class U = T>
		std::enable_if_t< IsVoid<U>, void> Call()
		{
			for (auto&& item : this->_functions)
			{
				item();
			}
		}
		template<class U = T>
		std::enable_if_t<!IsVoid<U>, std::vector<T>> Call()
		{
			std::vector<T> ret;
			ret.resize(this->_functions.size());

			const auto count = this->_functions.size();
			for (auto L10 = 0; L10 < count; L10++)
			{
				ret[L10] = this->_functions[L10]();
			}

			return ret;
		}

	private:
		std::vector<FunctionT> _functions;
	};

	template<class T, class ...Args>
	class Event<T(Args...)>
	{
		using FunctionT = std::function<T(Args...)>;

		template<class U>
		static constexpr bool IsVoid = std::is_same_v<U, void>;

	public:
		void Add(FunctionT&& action)
		{
			this->_functions.push_back(std::move(action));
		}
		void Clear()
		{
			this->_functions.clear();
		}
		int Count()const
		{
			return (int)this->_functions.size();
		}

		template<class U = T>
		std::enable_if_t< IsVoid<U>, void> Call(const Args& ...args)
		{
			for (auto&& item : this->_functions)
			{
				item(std::forward<Args>(args)...);
			}
		}
		template<class U = T>
		std::enable_if_t<!IsVoid<U>, std::vector<T>> Call(const Args& ...args)
		{
			std::vector<T> ret;
			ret.resize(this->_functions.size());

			const auto count = (int)this->_functions.size();
			for (auto L10 = 0; L10 < count; L10++)
			{
				ret[L10] = this->_functions[L10](std::forward<Args>(args)...);
			}

			return ret;
		}
	private:
		std::vector<FunctionT> _functions;

	};
}