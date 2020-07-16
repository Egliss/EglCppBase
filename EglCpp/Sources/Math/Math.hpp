#pragma once

#include <cmath>
#include <cstring>

namespace Egliss
{
	class Math
	{
	public:
		template<class T = float>
		static constexpr T PI = static_cast<T>(3.14159265358979323846264338327950288419716939);

		template<class T>
		constexpr static bool IsZero(T v)
		{
			return v == static_cast<T>(0);
		}
		template<class T>
		constexpr static T Degree(T radians)
		{
			return radians * 180.0f / PI<T>;
		}
		template<class T>
		constexpr static T Radians(T degree)
		{
			return degree / 180.0f * PI<T>;
		}
		template<class T>
		constexpr static T Lerp(T x, T x2, T time)
		{
			return (static_cast<T>(1) - time) * x + time * x2;
		}
		template<class T>
		static T LerpTime(T min, T max, T value)
		{
			const auto v = value - min;
			const auto distance = Math::Abs(max - min);
			return v / distance;
		}
		template<class T>
		constexpr static T Clamp(T value, T min = 0, T max = 1)
		{
			return
				value < min
				? min
				: value > max
				? max
				: value;
		}
		template<class T>
		constexpr static T Repeat(T value, T min = 0, T max = 1)
		{
			while (value < min)
				value += (max - min);
			while (value > max)
				value -= (max - min);
			return value;
		}
		template<class T>
		constexpr static T Saturate(T value)
		{
			return Clamp(value, 0.0f, 1.0f);
		}
		template<class T>
		constexpr static T Directional(T value)
		{
			if (value < 0.0f)
				return -1.0f;
			else if (value > 0.0f)
				return 1.0f;
			return 0.0f;
		}

		//Quated by http://www.cs.uni.edu/~jacobson/C++/newton.html
		//only use constexpr !!
		//the function is slowly...
		static constexpr float Sqrt(float number)
		{
			const double ACCURACY = 0.001;
			double lower = 0, upper = 0, guess = 0;

			if (number < 1)
			{
				lower = number;
				upper = 1;
			}
			else
			{
				lower = 1;
				upper = number;
			}

			while ((upper - lower) > ACCURACY)
			{
				guess = (lower + upper) / 2;
				if (guess * guess > number)
					upper = guess;
				else
					lower = guess;
			}
			return (float)(lower + upper) / 2;
		}
	};
}