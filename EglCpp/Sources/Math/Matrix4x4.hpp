#pragma once

#include "Math.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

#include "XMath.hpp"
#include <DirectXMath.h>

namespace Egliss
{
	class Matrix4x4
	{
		using XMF3 = ::DirectX::XMFLOAT3;
		using XMF4 = ::DirectX::XMFLOAT4;
		using XMF44 = ::DirectX::XMFLOAT4X4;
		using XMV4 = ::DirectX::XMVECTOR;
		using XMMat = ::DirectX::XMMATRIX;
	public:
		Matrix4x4() :Matrix4x4(Matrix4x4::Identity) {}
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		) :
			m00(m00), m01(m01), m02(m02), m03(m03),
			m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23),
			m30(m30), m31(m31), m32(m32), m33(m33)
		{
		}
		Matrix4x4(const Matrix4x4& matrix)
		{
			*this = matrix;
		}
		Matrix4x4(Matrix4x4&& matrix)
		{
			*this = static_cast<Matrix4x4&&>(matrix);
		}
		Matrix4x4& operator=(const Matrix4x4& matrix)
		{
			std::memcpy(&m00, &matrix, sizeof(Matrix4x4));
			return *this;
		}
		Matrix4x4& operator=(Matrix4x4&& matrix)
		{
			std::memcpy(&m00, &matrix, sizeof(Matrix4x4));
			return *this;
		}
		static const Matrix4x4 Identity;

		Matrix4x4 operator*(const Matrix4x4& matrix)const
		{
			XMMat m1 = ::DirectX::XMLoadFloat4x4((const XMF44*)this);
			XMMat m2 = ::DirectX::XMLoadFloat4x4((const XMF44*)&matrix);
			XMMat m3 = ::DirectX::XMMatrixMultiply(m1, m2);
			Matrix4x4 m;
			::DirectX::XMStoreFloat4x4((XMF44*)&m, m3);
			return m;
		}
		Vector4 operator*(const Vector4& v)const
		{
			XMMat m1 = ::DirectX::XMLoadFloat4x4((const XMF44*)this);
			XMV4 m2 = ::DirectX::XMLoadFloat4((const XMF4*)(&v));
			XMV4 m3 = ::DirectX::XMVector4Transform(m2, m1);
			Vector4 m4;
			::DirectX::XMStoreFloat4((XMF4*)&m4, m3);
			return m4;
		}
		Vector3 operator*(const Vector3& v)const
		{
			Vector4 dest = Vector4(v.x, v.y, v.z, 1.0f);
			XMMat m1 = ::DirectX::XMLoadFloat4x4((const XMF44*)this);
			XMV4 m2 = ::DirectX::XMLoadFloat4((const XMF4*)&dest);
			XMV4 m3 = ::DirectX::XMVector4Transform(m2, m1);
			Vector3 m4;
			::DirectX::XMStoreFloat3((XMF3*)&m4, m3);
			return m4;
		}

		Matrix4x4& Inverse()
		{
			XMMat m1 = ::DirectX::XMLoadFloat4x4((const XMF44*)this);
			//sńŽÍł
			XMMat m3 = ::DirectX::XMMatrixInverse(nullptr, m1);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m3);
			return *this;
		}
		Matrix4x4& Transpose()
		{
			XMMat m1 = ::DirectX::XMLoadFloat4x4((const XMF44*)this);
			XMMat m3 = ::DirectX::XMMatrixTranspose(m1);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m3);
			return *this;
		}
		Matrix4x4& FromEuler(float x, float y, float z)
		{
			XMMat m1 = ::DirectX::XMMatrixRotationRollPitchYaw(Math::Radians(x), Math::Radians(y), Math::Radians(z));
			::DirectX::XMStoreFloat4x4((XMF44*)this, m1);
			return *this;
		}
		Matrix4x4& AxisAngle(const Vector3& axis, float angle)
		{
			XMV4 m0 = ::DirectX::XMLoadFloat3((const XMF3*)&axis);
			XMMat m1 = ::DirectX::XMMatrixRotationAxis(m0, Math::Radians(angle));
			::DirectX::XMStoreFloat4x4((XMF44*)this, m1);
			return *this;
		}
		Matrix4x4& View(const Vector3& position, const Vector3& forward, const Vector3& up)
		{
			XMV4 v0 = ::DirectX::XMLoadFloat3((const XMF3*)&position);
			XMV4 v1 = ::DirectX::XMLoadFloat3((const XMF3*)&forward);
			XMV4 v2 = ::DirectX::XMLoadFloat3((const XMF3*)&up);

			XMMat m = ::DirectX::XMMatrixLookToLH(v0, v1, v2);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m);
			return *this;
		}
		Matrix4x4& FromAffine(const Vector3& pos, const Vector3& euler, const Vector3& scaling)
		{
			Vector3 eu = Vector3(Math::Radians(euler.x), Math::Radians(euler.y), Math::Radians(euler.z));
			XMV4 v0 = ::DirectX::XMLoadFloat3((const XMF3*)&pos);
			XMV4 v1 = ::DirectX::XMLoadFloat3((const XMF3*)&eu);
			XMV4 v2 = ::DirectX::XMLoadFloat3((const XMF3*)&scaling);
			XMV4 v3 = ::DirectX::XMQuaternionRotationRollPitchYawFromVector(v1);
			XMMat m0 = ::DirectX::XMMatrixAffineTransformation(v2, ::DirectX::g_XMZero, v3, v0);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m0);
			return *this;
		}
		Matrix4x4& FromAffine(const Vector3& pos, const Quaternion& quat, const Vector3& scaling)
		{
			XMV4 v0 = ::DirectX::XMLoadFloat3((const XMF3*)&pos);
			XMV4 v1 = ::DirectX::XMLoadFloat4((const XMF4*)&quat);
			XMV4 v2 = ::DirectX::XMLoadFloat3((const XMF3*)&scaling);
			XMMat m0 = ::DirectX::XMMatrixAffineTransformation(v2, ::DirectX::g_XMZero, v1, v0);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m0);
			return *this;
		}
		Matrix4x4& FromRotate(const Quaternion& quat)
		{
			XMV4 v0 = ::DirectX::XMLoadFloat4((const XMF4*)&quat);
			XMMat m0 = ::DirectX::XMMatrixRotationQuaternion(v0);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m0);
			return *this;
		}
		Vector3 Position()const
		{
			return Vector3(this->m30, this->m31, this->m32);
		}
		Vector3 Right()const
		{
			return this->Rotation() * Vector3(1.0f, 0.0f, 0.0f);
		}
		Vector3 Up()const
		{
			return this->Rotation() * Vector3(0.0f, 1.0f, 0.0f);
		}
		Vector3 Forward()const
		{
			return this->Rotation() * Vector3(0.0f, 0.0f, 1.0f);
		}
		Vector3 Scale()const
		{
			__m128 t;
			__m128 r;
			__m128 s;

			::DirectX::XMMatrixDecompose(&s, &r, &t, ToXMMat());
			Vector3 res;
			XMath::Store3(&res.x, s);
			return res;
		}
		Quaternion Rotation()const
		{
			__m128 rot = ::DirectX::XMQuaternionRotationMatrix(ToXMMat());
			Quaternion quat;
			XMath::Store4(&quat.x, rot);
			return quat;
		}
		Matrix4x4& Projection(float fovY, float aspectRate, float near_, float far_)
		{
			XMMat m = ::DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRate, near_, far_);
			::DirectX::XMStoreFloat4x4((XMF44*)this, m);
			return *this;
		}
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;

	private:
		__forceinline ::DirectX::XMMATRIX ToXMMat()const
		{
			return ::DirectX::XMLoadFloat4x4((XMF44*)this);
		}
		__forceinline Matrix4x4& __vectorcall operator=(::DirectX::XMMATRIX m)
		{
			::DirectX::XMStoreFloat4x4((XMF44*)&this->m00, m);
			return *this;
		}

	};
}