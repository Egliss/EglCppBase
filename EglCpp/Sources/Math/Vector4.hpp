#pragma once

namespace Egliss
{
	struct Vector4 final
	{
	public:
		static const Vector4 Zero;		//(0.0f,0.0f,0.0f,0.0f)
		static const Vector4 WOne;		//(0.0f,0.0f,0.0f,1.0f)

		float x;
		float y;
		float z;
		float w;

		constexpr Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) :x(x), y(y), z(z), w(w) {}
		constexpr Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		constexpr Vector4(const Vector3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
		~Vector4() = default;

		constexpr Vector4& operator=(const Vector3& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = 1.0f;
			return *this;
		}

		Vector3 AsVector3()const
		{
			return Vector3(x, y, z);
		}

		Vector4 operator/(float value)
		{
			Vector4 t = Vector4(*this);
			t /= value;
			return t;
		}
		void operator/=(float value)
		{
			this->x /= value;
			this->y /= value;
			this->z /= value;
			this->w /= value;
		}
		Vector4& operator+=(const Vector4& vec)
		{
			this->x += vec.x;
			this->y += vec.y;
			this->z += vec.z;
			this->w += vec.w;
			return *this;
		}
		Vector4& operator*=(const Vector4& vec)
		{
			this->x *= vec.x;
			this->y *= vec.y;
			this->z *= vec.z;
			this->w *= vec.w;
			return *this;
		}
		Vector4& operator*=(const Vector3& vec)
		{
			this->x *= vec.x;
			this->y *= vec.y;
			this->z *= vec.z;
			return *this;
		}
		Vector4& operator*=(float scaler)
		{
			this->x *= scaler;
			this->y *= scaler;
			this->z *= scaler;
			this->w *= scaler;
			return *this;
		}

		Vector4& MultiplyV3(float scaling)
		{
			this->x *= scaling;
			this->y *= scaling;
			this->z *= scaling;
			return *this;
		}

		bool IsZero() const
		{
			if (std::memcmp((const void*)this, (const void*)&Vector4::Zero, sizeof(Vector4)) == 0)return true;
			return false;
		}
		float Length() const
		{
			return std::sqrt(
				(this->x * this->x) +
				(this->y * this->y) +
				(this->z * this->z) +
				(this->w + this->w)
			);
		}
		Vector4& Normalize()
		{
			*this /= Length();
			return *this;
		}
	};
}