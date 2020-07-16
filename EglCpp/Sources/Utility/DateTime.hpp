#pragma once

#include <string>
#include <ctime>

namespace Egliss
{
	class DateTime final
	{
	public:
		DateTime() : _time(std::time_t(0))
		{

		}
		static DateTime Now();

		static DateTime FromSecond(int second);
		static DateTime FromMinute(int minute);
		static DateTime FromHour(int hour);
		static DateTime FromDay(int day);
		static DateTime FromMonth(int month);
		// in +1900
		static DateTime FromLocalYear(int year);
		// in +0
		static DateTime FromGlobalYear(int year);

		DateTime operator+(const DateTime& time)const
		{
			return DateTime(*this) += time;
		}
		DateTime& operator+=(const DateTime& time)
		{
			this->_time += time._time;
			return *this;
		}
		DateTime operator-(const DateTime& time)const
		{
			return DateTime(*this) -= time;
		}
		DateTime& operator-=(const DateTime& time)
		{
			this->_time -= time._time;
			return *this;
		}
		bool operator==(const DateTime& time)
		{
			return this->_time == time._time;
		}

		int Seconds()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_sec;
		}
		int Minute()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_min;
		}
		int Hour()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_hour;
		}
		// [0..6]
		int Week()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_wday;
		}
		int Month()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_mon;
		}
		int Year()const
		{
			std::tm t;
			localtime_s(&t, &this->_time);
			return t.tm_year;
		}

		// http://www.cplusplus.com/reference/iomanip/put_time/
		std::string Format(const char* format = "%Y-%m-%d %H:%M:%S") const;
	private:

		std::time_t _time;
	};
}