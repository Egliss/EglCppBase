#include "pch.h"

#include "Math/Math.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"

using namespace Egliss;

const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion& Quaternion::FromMatrix(const Matrix4x4& matrix)
{
	*this = matrix.Rotation();
	return *this;
}

Quaternion& Quaternion::ToWards(const Quaternion& toQuat, float degreeStep)
{
	Vector3 from = *this * Vector3::Forward;
	Vector3 to = toQuat * Vector3::Forward;
	from.Normalize();

	float ret = from.Dot(to);
	if (ret >= 0.99f)
	{
		*this = toQuat;
		return *this;
	}

	Vector3 crossed = from;
	crossed.Cross(to);

	//Apply rotate
	*this *= Quaternion().FromAxisAngle(from, degreeStep);
	Vector3 retV = *this * Vector3::Forward;

	float ret2 = retV.Dot(to);
	if (ret2 < ret || ret >= 0.99f)
	{
		*this = toQuat;
	}
	return *this;
}

Vector3 Quaternion::ToEulerAngle() const
{
	Vector3 output;
	// roll (x-axis rotation)
	float sinr_cosp = +2.0f * (w * x + y * z);
	float cosr_cosp = +1.0f - 2.0f * (x * x + y * y);
	output.x = Math::Degree(std::atan2(sinr_cosp, cosr_cosp));

	// pitch (y-axis rotation)
	float sinp = +2.0f * (w * y - z * x);
	if (fabs(sinp) >= 1)
		// use 90 degrees if out of range
		output.y = Math::Degree((float)std::copysign(Math::PI<float> / 2, sinp));
	else
		output.y = Math::Degree(std::asin(sinp));

	// yaw (z-axis rotation)
	float siny_cosp = +2.0f * (w * z + x * y);
	float cosy_cosp = +1.0f - 2.0f * (y * y + z * z);
	output.z = Math::Degree(std::atan2(siny_cosp, cosy_cosp));

	return output;
}
