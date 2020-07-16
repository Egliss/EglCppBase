#pragma once

#include "Math.hpp"
#include "XMath.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <DirectXMath.h>

#pragma warning(push)
#pragma warning(disable: 4458)

namespace Egliss
{
	class Matrix4x4;

	class Quaternion
	{
	public:
		static const Quaternion Identity;

		Quaternion() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};
		Quaternion(float x, float y, float z, float w = 1.0f) :
			x(x),
			y(y),
			z(z),
			w(w)
		{
		}
		Quaternion(const Quaternion& quat)
		{
			*this = quat;
		}
		Quaternion& operator=(const Quaternion& quat)
		{
			std::memcpy(&this->x, &quat.x, sizeof(Quaternion));
			return *this;
		}

		Quaternion operator*(const Quaternion& quat)const
		{
			return Quaternion(*this).Multiply(quat);
		}
		Quaternion& operator*=(const Quaternion& quat)
		{
			return this->Multiply(quat);
		}
		Vector3 operator*(const Vector3& direction)const
		{
			return this->Multiply(direction);
		}
		Vector4 operator*(const Vector4& direction)const
		{
			return this->Multiply(direction);
		}
		Quaternion& FromEuler(float x, float y, float z)
		{
			return FromEuler(Vector3(x, y, z));
		}
		Quaternion& FromXAxis(float x)
		{
			auto v = Math::Radians(x) * 0.5f;
			float c = std::cos(v);
			float s = std::sin(v);
			*this = { s, 0.0f, 0.0f, c };
			return *this;
		}
		Quaternion& FromYAxis(float y)
		{
			auto v = Math::Radians(y) * 0.5f;
			float c = std::cos(v);
			float s = std::sin(v);
			*this = { 0.0f, s, 0.0f, c };
			return *this;
		}
		Quaternion& FromZAxis(float z)
		{
			auto v = Math::Radians(z) * 0.5f;
			float c = std::cos(v);
			float s = std::sin(v);
			*this = { 0.0f, 0.0f, s, c };
			return *this;
		}
		Quaternion& FromEuler(const Vector3& euler)
		{
			__m128 e1 = XMath::Load3(&euler.x);
			__m128 e2 = XMath::ToRadians(e1);
			__m128 quat = ::DirectX::XMQuaternionRotationRollPitchYawFromVector(e2);
			XMath::Store4(&this->x, quat);
			return *this;
		}
		Quaternion& FromAxisAngle(const Vector3& axis, float angle)
		{
			__m128 e1 = XMath::Load3(&axis.x);
			__m128 quat = ::DirectX::XMQuaternionRotationAxis(e1, Math::Radians(angle));
			XMath::Store4(&this->x, quat);
			return *this;
		}
		Quaternion& FromMatrix(const Matrix4x4& matrix);
		Quaternion& FromDirection(const Vector3& forward)
		{
			const auto dir = forward.Normalized();
			Vector3 axis = Vector3(Vector3::Forward).Cross(dir);
			float dot = Vector3::Forward.Dot(forward);

			::DirectX::XMVECTOR v = { axis.x,axis.y,axis.z,dot + 1.0f };

			XMath::Store4(&this->x, ::DirectX::XMQuaternionNormalize(v));
			return *this;
		}
		Quaternion& Multiply(const Quaternion& quat)
		{
			__m128 m1 = XMath::Load4(&this->x);
			__m128 m2 = XMath::Load4(&quat.x);
			__m128 m3 = ::DirectX::XMQuaternionMultiply(m1, m2);
			XMath::Store4(&this->x, m3);
			return *this;
		}
		Quaternion& Slerp(const Quaternion& q1, const Quaternion& q2, float time)
		{
			__m128 res = ::DirectX::XMQuaternionSlerp(__M128(q1), __M128(q2), time);
			XMath::Store4(&this->x, res);
			return *this;
		}
		Vector3 Multiply(const Vector3& direction)const
		{
			__m128 rot = __M128(*this);
			__m128 dir = XMath::Load3(&direction.x);
			__m128 res = ::DirectX::XMVector3Rotate(dir, rot);
			Vector3 v;
			XMath::Store3(&v.x, res);
			return v;
		}
		Vector4 Multiply(const Vector4& direction)const
		{
			__m128 rot = __M128(*this);
			__m128 dir = XMath::Load3(&direction.x);
			__m128 res = ::DirectX::XMVector3Rotate(dir, rot);
			Vector4 v;
			XMath::Store3(&v.x, res);
			v.w = 1.0f;
			return v;
		}
		Quaternion& Inverse()
		{
			__m128 quat = __M128(*this);
			__m128 res = ::DirectX::XMQuaternionInverse(quat);
			XMath::Store4(&this->x, res);
			return *this;
		}
		Quaternion& ToWards(const Quaternion& toQuat, float degreeStep);

		float Dot(const Quaternion& quat) const
		{
			return
				this->x * quat.x +
				this->y * quat.y +
				this->z * quat.z +
				this->w * quat.w;
		}

		//Quat by https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		Vector3 ToEulerAngle() const;

		static __m128 __forceinline __vectorcall __M128(const Quaternion& quat)
		{
			return XMath::Load4(&quat.x);
		}


		float x, y, z, w;
	};
}
#pragma warning(pop)
