#pragma once

#include <type_traits>
#include <vector>
#include <functional>

namespace Egliss
{
	template<class T>
	class IEvent
	{
	};
	// not interface
	// Event's public method only class
	template<class T, class ...Args>
	class IEvent<T(Args ...)>
	{
		using FunctionT = std::function<T(Args ...)>;
	public:
		void inline Add(FunctionT&& action)
		{
			static_cast<T&>(this)->Add(std::move(action));
		}
		void inline Clear()
		{
			static_cast<T&>(this)->Clear();
		}
		int inline Count()const
		{
			return static_cast<T&>(this)->Count();
		}
	};

	/// <summary>
	/// This is invalid event.<para/>
	/// Please input type -> T() or T(ArgT)
	/// </summary>
	template<class T>
	class Event
	{
	};

	template<class T, class ...Args>
	class Event<T(Args ...)> : public IEvent<T(Args ...)>
	{
		using FunctionT = std::function<T(Args ...)>;

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
		IEvent<T(Args ...)> AsIEvent()
		{
			return *this;
		}
		template<class U = T>
		std::enable_if_t< IsVoid<U>, void> Call(Args ...args)
		{
			for (auto&& item : this->_functions)
			{
				item(std::forward<Args>(args)...);
			}
		}
		template<class U = T>
		std::enable_if_t<!IsVoid<U>, std::vector<T>> Call(Args ...args)
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