#ifndef ND_MATH_HPP
#define ND_MATH_HPP

#include "units/angle.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

namespace nd::math
{

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> translation(const Vector3<ValueType> vector)
{
	return Matrix4x4<ValueType>{{
		1, 0, 0, vector[0u],
		0, 1, 0, vector[1u],
		0, 0, 1, vector[2u],
		0, 0, 0,          1
	}};
}

///
/// Returns a view matrix calculated from the camera position \a eye, the point at which the camera shall look \a center and the upwards direction \a up, which
///	usually is $(0, 1, 0)$.
///
template <typename ValueType>
inline constexpr Matrix4x4<ValueType> lookAt(const Vector3<ValueType> eye, const Vector3<ValueType> center, const Vector3<ValueType> up = {0, 1, 0})
{
	Matrix4x4<ValueType> returnValue;

	const Vector3<ValueType>	cameraDirection		= (eye - center).normalized();
	const Vector3<ValueType>	cameraRight			= up.cross(cameraDirection).normalized();
	const Vector3<ValueType>	cameraUp			= cameraDirection.cross(cameraRight); // already normalized due to direction & right being normalized

	const Matrix4x4<ValueType>	cameraRotation		= {
	   Vector4<ValueType>{cameraRight},
	   Vector4<ValueType>{cameraUp},
	   Vector4<ValueType>{cameraDirection},
	   {0, 0, 0, 1}
	};

	const Matrix4x4<ValueType>	cameraTranslation	= {
	   1, 0, 0, -eye[0u],
	   0, 1, 0, -eye[1u],
	   0, 0, 1, -eye[2u],
	   0, 0, 0,        0
	};

	returnValue = cameraRotation * cameraTranslation;

	return returnValue;
}

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> perspective(const units::Radians<ValueType> fieldOfView, const ValueType aspectRatio, const ValueType near,
												  const ValueType far)
{
	Matrix4x4<ValueType> returnValue;

	using std::tan;
	const ValueType scaling0	= static_cast<ValueType>(1) / tan(fieldOfView / static_cast<ValueType>(2));
	const ValueType scaling1	= scaling0 / aspectRatio;
	const ValueType factor0		= (near + far) / (near - far);
	const ValueType factor1		= (static_cast<ValueType>(2) * near * far) / (near - far);

	returnValue = {
		scaling1,        0,       0,       0,
			   0, scaling0,       0,       0,
			   0,        0, factor0,       0,
			   0,        0,       0, factor1,
			   0,        0,      -1,       0
	};

	return returnValue;
}

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> perspective(const ValueType fieldOfView, const ValueType width, const ValueType height, const ValueType near,
												  const ValueType far)
{
	return perspective(fieldOfView, (width / height), near, far);
}

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> rotation(const units::Radians<ValueType> x, const units::Radians<ValueType> y, const units::Radians<ValueType> z)
{
	Matrix4x4<ValueType> returnValue;

	const Quaternion<ValueType> rotationX{x, {1, 0, 0}};
	const Quaternion<ValueType> rotationY{y, {0, 1, 0}};
	const Quaternion<ValueType> rotationZ{z, {0, 0, 1}};

	returnValue = (rotationX * rotationY * rotationZ).toRotationMatrix();

	return returnValue;
}

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> &rotation(Matrix4x4<ValueType> &rotationMatrix, const units::Radians<ValueType> x, const units::Radians<ValueType> y,
												const units::Radians<ValueType> z)
{
	return rotationMatrix *= rotation(x, y, z).toRotationMatrix();
}

template <typename ValueType>
inline constexpr Matrix4x4<ValueType> scaling(const Vector3<ValueType> vector)
{
	return Matrix4x4<ValueType>{{
		vector[0u],          0,          0, 0,
				 0, vector[1u],          0, 0,
				 0,          0, vector[2u], 0,
				 0,          0,          0, 1
	}};
}

}

#endif // ND_MATH_HPP
