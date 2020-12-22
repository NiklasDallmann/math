#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <matrix.hpp>
#include <quaternion.hpp>
#include <units.hpp>

#include "test.hpp"

using namespace nd::math;

int main(int, char **)
{
//	{
//		Quaternion_f q{1.0f, 1.0f, 1.0f, 1.0f};

//		const Quaternion_f q_ = q.inverted();

//		std::cout << q << "\n";
//		std::cout << q_ << "\n";
//	}

//	{
//		using namespace nd::math::literals;

//		Quaternion_f q{0.5_rad, Vector3_f{{1.0f, 0.0f, 0.0f}}};

//		std::cout << q << " " << q.inverted() << "\n";
//	}

	{
		constexpr Quaternion_f q0{1.0f, 2.0f, 3.0f, 4.0f};
		constexpr Quaternion_f q1{5.0f, 6.0f, 7.0f, 8.0f};
		Quaternion_f r = q0 * q1;

		std::cout << r << "\n";
	}

	{
		using namespace nd::math::units::literals;

		Quaternion_f	q{M_PI * 0.5_rad, Vector3_f{{1.0f, 0.0f, 0.0f}}};
		Vector4_f		v{{0.0f, 0.0f, 1.0f, 1.0f}};

		Vector3_f		r = q * Quaternion_f{v} * q.inverted();

		std::cout << q << " " << q.inverted() << " " << r << "\n";
	}

	return EXIT_SUCCESS;
}
