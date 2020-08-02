#pragma once

#include "Math.hpp"
#include "Vector2.hpp"

#pragma warning(push)
#pragma warning(disable: 4458)

namespace Egliss
{
	struct Vector4;
	struct Vector3 final
	{
	public:

		static const Vector3 Up;		//(0.0f,1.0f,0.0f)
		static const Vector3 Down;		//(0.0f,-1.0f,0.0f)
		static const Vector3 Forward;	//(0.0f,0.0f,1.0f)
		static const Vector3 Back;		//(0.0f,0.0f,-1.0f)
		static const Vector3 Right;		//(1.0f,0.0f,0.0f)
		static const Vector3 Left;		//(-1.0f,0.0f,0.0f)
		static const Vector3 Zero;		//(0.0f,0.0f,0.0f)
		static const Vector3 ScaleOne;	//(1.0f,1.0f,1.0f)

		float x;
		float y;
		float z;

		constexpr Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) :x(x), y(y), z(z) {}
		constexpr Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
		explicit Vector3(const Vector4& vec);

		Vector3& operator=(const Vector3& vec)
		{
			std::memcpy(&this->x, &vec, sizeof(Vector3));
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			return *this;
		}
		Vector3& operator+=(const Vector3& vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		Vector3& operator*=(float val)
		{
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}
		constexpr Vector3& operator/=(float val)
		{
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}
		Vector3& operator*=(const Vector3& vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}
		constexpr Vector3& operator/=(const Vector3& vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}
		Vector3 operator-() const
		{
			return *this * -1.0f;
		}
		Vector3 operator+(const Vector3& vec) const
		{
			Vector3 n = *this;
			return (n += vec);
		}
		Vector3 operator-(const Vector3& vec) const
		{
			Vector3 n = *this;
			return n -= vec;
		}
		Vector3 operator*(float val) const
		{
			Vector3 v = *this;
			v *= val;
			return v;
		}
		constexpr Vector3 operator/(float val)const
		{
			Vector3 v = *this;
			v /= val;
			return v;
		}
		Vector3 operator*(const Vector3& vec) const
		{
			Vector3 v = *this;
			v *= vec;
			return v;
		}
		constexpr Vector3 operator/(const Vector3& vec)const
		{
			Vector3 v = *this;
			v /= vec;
			return v;
		}

		bool IsZero() const
		{
			if (std::memcmp((const void*)this, (const void*)&Vector3::Zero, sizeof(Vector3)) == 0)
				return true;
			return false;
		}
		constexpr float Dot(const Vector3& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z;
		}
		float Distance(const Vector3& vec) const
		{
			return (*this - vec).Length();
		}
		float Length()const
		{
			return std::hypot(x, y, z);
		}
		constexpr Vector3 Normalized()const
		{
			return *this / this->Length();
		}

		Vector3& SetX(float x)
		{
			this->x = x;
			return *this;
		}
		Vector3& SetY(float y)
		{
			this->y = y;
			return *this;
		}
		Vector3& SetZ(float z)
		{
			this->z = z;
			return *this;
		}
		Vector3& SetXYZ(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			return *this;
		}

		Vector3& Clamp(float min, float max)
		{
			Math::Clamp(this->x, min, max);
			Math::Clamp(this->y, min, max);
			Math::Clamp(this->z, min, max);
			return *this;
		}
		Vector3& Repeat(float min, float max)
		{
			Math::Repeat(this->x, min, max);
			Math::Repeat(this->y, min, max);
			Math::Repeat(this->z, min, max);
			return *this;
		}
		Vector3& Clamp(const Vector3& min, const Vector3& max)
		{
			Math::Clamp(this->x, min.x, max.x);
			Math::Clamp(this->y, min.y, max.y);
			Math::Clamp(this->z, min.z, max.z);
			return *this;
		}
		Vector3& Repeat(const Vector3& min, const Vector3& max)
		{
			Math::Repeat(this->x, min.x, max.x);
			Math::Repeat(this->y, min.y, max.y);
			Math::Repeat(this->z, min.z, max.z);
			return *this;
		}

		Vector3& Cross(const Vector3& vec)
		{
			Vector3 m1 = *this;
			this->x = m1.y * vec.z - m1.z * vec.y;
			this->y = m1.z * vec.x - m1.x * vec.z;
			this->z = m1.x * vec.y - m1.y * vec.x;
			return *this;
		}
		Vector3& Normalize()
		{
			*this /= this->Length();
			return *this;
		}
		Vector3& Multiply(const Vector3& vec)
		{
			this->x *= vec.x;
			this->y *= vec.y;
			this->z *= vec.z;
			return *this;
		}
		Vector3& Slide(const Vector3& normal)
		{
			*this = (*this - normal * Dot(normal));
			return *this;
		}

		Vector3& Lerp(const Vector3& v, const Vector3& v2, float time)
		{
			this->x = Math::Lerp(v.x, v2.x, time);
			this->y = Math::Lerp(v.y, v2.y, time);
			this->z = Math::Lerp(v.z, v2.z, time);
			return *this;
		}

		Vector3& EulerAngleToDirection();
	};
}

#pragma warning(pop)
