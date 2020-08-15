#pragma once

#include <type_traits>
#include <vector>
#include <functional>

namespace Egliss
{
	// not interface
	// Event's public method only class
	template<class EventT, class FunctionT>
	class IEvent
	{
	public:
		void inline Add(FunctionT&& action)
		{
			const_cast<EventT*>(static_cast<const EventT*>(this))->Add(std::move(action));
		}
		void inline Clear()
		{
			const_cast<EventT*>(static_cast<const EventT*>(this))->Clear();
		}
		int inline Count()const
		{
			return static_cast<const EventT*>(this)->Count();
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
	class Event<T(Args ...)> : public IEvent<Event<T(Args ...)>, std::function<T(Args ...)>>
	{
		template<class U>
		static constexpr bool IsVoid = std::is_same_v<U, void>;
	public:
		using FunctionT = std::function<T(Args ...)>;
		using IEventT = IEvent<Event<T(Args ...)>, std::function<T(Args ...)>>;

		void Add(FunctionT&& action)
		{
			this->_functions.emplace_back(std::move(action));
		}
		void Clear()
		{
			this->_functions.clear();
		}
		int Count()const
		{
			return (int)this->_functions.size();
		}
		IEventT& AsIEvent()
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