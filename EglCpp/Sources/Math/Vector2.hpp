#pragma once

#include "Math.hpp"

#pragma warning(push)
#pragma warning(disable: 4458)

namespace Egliss
{
	struct Vector2 final
	{
	public:
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Right;
		static const Vector2 Left;
		static const Vector2 Zero;
		static const Vector2 OneZero;
		static const Vector2 ZeroOne;
		static const Vector2 OneOne;

		float x;
		float y;

		constexpr Vector2(float x = 0.0f, float y = 0.0f) :x(x), y(y) {}
		constexpr Vector2(const Vector2& v) : x(v.x), y(v.y) {}

		~Vector2() = default;

		Vector2 operator+() const
		{
			return Vector2(x, y);
		}
		Vector2 operator-() const
		{
			return Vector2(x, y) * -1.0f;
		}
		Vector2 operator+(const Vector2& vec) const
		{
			return Vector2(x + vec.x, y + vec.y);
		}
		Vector2 operator-(const Vector2& vec) const
		{
			return Vector2(x - vec.x, y - vec.y);
		}
		Vector2& operator+=(const Vector2& vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}
		Vector2& operator-=(const Vector2& vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}
		Vector2 operator*(float value) const
		{
			return Vector2(x * value, y * value);
		}
		Vector2 operator/(float value) const
		{
			return Vector2(x / value, y / value);
		}

		Vector2& operator*=(float value)
		{
			x *= value;
			y *= value;
			return *this;
		}
		Vector2& operator/=(float value)
		{
			x /= value;
			y /= value;
			return *this;
		}
		Vector2& operator/=(const Vector2& vec)
		{
			x /= vec.x;
			y /= vec.y;
			return *this;
		}
		bool operator==(const Vector2& vec) const
		{
			if (x == vec.x && y == vec.y)
				return true;
			return false;
		}
		bool operator!=(const Vector2& vec) const
		{
			if (x != vec.x || y != vec.y)
				return true;
			return false;
		}
		Vector2 operator*(const Vector2& vec) const
		{
			return Vector2(x * vec.x, y * vec.y);
		}
		Vector2 operator/(const Vector2& vec) const
		{
			return Vector2(x / vec.x, y / vec.y);
		}

		bool IsZero()const
		{
			if (std::memcmp((const void*)this, (const void*)&Vector2::Zero, sizeof(Vector2)) == 0)
				return true;
			return false;
		}
		float Length() const
		{
			return std::hypot(x, y);
		}
		float Rot() const
		{
			return Math::Degree(std::atan2(x, y));
		}
		Vector2 Normalized() const
		{
			Vector2 e = *this;
			return e.Normalize();
		}

		Vector2& SetX(float x)
		{
			this->x = x;
			return *this;
		}
		Vector2& SetY(float y)
		{
			this->y = y;
			return *this;
		}
		Vector2& SetXY(float x, float y)
		{
			this->x = x;
			this->y = y;
			return *this;
		}
		Vector2& Normalize()
		{
			*this /= Length();
			return *this;
		}

		[[deprecated("PleaseTest")]]
		Vector2& AddRotate(float radian)
		{
			radian += (std::atan2(x, y));
			float _srad = std::cosf(radian);
			float _crad = std::sinf(radian);
			float _x = _crad * x - _srad * y;
			float _y = _srad * x + _crad * y;
			*this = Vector2(_x, _y);
			return *this;
		}
		Vector2& SetRotate(const Vector2& vec, float radian)
		{
			float _srad = std::cosf(radian);
			float _crad = std::sinf(radian);
			float _x = _crad * vec.x - _srad * vec.y;
			float _y = _srad * vec.x + _crad * vec.y;
			*this = Vector2(_x, _y);
			return *this;
		}
	};

}

#pragma warning(pop)