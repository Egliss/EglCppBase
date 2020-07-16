#include "pch.h"
#include "DateTime.hpp"
#include <sstream>
#include <iomanip>

using namespace Egliss;

DateTime DateTime::Now()
{
	//now time
	time_t now = time(nullptr);

	DateTime date;
	date._time = now;
	return date;
}

DateTime DateTime::FromSecond(int second)
{
	DateTime t;
	std::tm local = { second, 0, 0, 0, 0, 0 };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromMinute(int minute)
{
	DateTime t;
	std::tm local = { 0, minute, 0, 0, 0, 0 };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromHour(int hour)
{
	DateTime t;
	std::tm local = { 0, 0, hour, 0, 0, 0 };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromDay(int day)
{
	DateTime t;
	std::tm local = { 0, 0, 0, day, 0, 0 };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromMonth(int month)
{
	DateTime t;
	std::tm local = { 0, 0, 0, 0, month, 0 };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromLocalYear(int year)
{
	DateTime t;
	std::tm local = { 0, 0, 0, 0, 0, year };
	t._time = std::mktime(&local);
	return t;
}

DateTime DateTime::FromGlobalYear(int year)
{
	DateTime t;
	std::tm local = { 0, 0, 0, 0, 0, year - 1900 };
	t._time = std::mktime(&local);
	return t;
}

std::string DateTime::Format(const char* format) const
{
	std::tm t;
	localtime_s(&t, &this->_time);
	std::stringstream stream;
	stream << std::put_time(&t, format);

	return stream.str();
}
