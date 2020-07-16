#include "pch.h"
#include "Math.hpp"
#include <DirectXMath.h>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

using namespace Egliss;

const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);
const Vector2 Vector2::Down = Vector2(0.0f, -1.0f);
const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
const Vector2 Vector2::Left = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
const Vector2 Vector2::OneZero = Vector2(1.0f, 0.0f);
const Vector2 Vector2::ZeroOne = Vector2(0.0f, 1.0f);
const Vector2 Vector2::OneOne = Vector2(1.0f, 1.0f);

const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::ScaleOne = Vector3(1.0f, 1.0f, 1.0f);

Vector3::Vector3(const Vector4& vec) : x(vec.x), y(vec.y), z(vec.z)
{

}

Vector3& Vector3::EulerAngleToDirection()
{
	const auto radX = Math::Radians(this->x);
	const auto radY = Math::Radians(this->y);
	const auto radZ = Math::Radians(this->z);
	DirectX::XMVECTOR s = DirectX::XMQuaternionRotationRollPitchYaw(radX, radY, radZ);
	DirectX::XMVECTOR v = DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&Vector3::Forward));
	DirectX::XMVECTOR r = DirectX::XMVector3Rotate(v, s);
	DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(this), r);
	return *this;
}

const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::WOne = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

