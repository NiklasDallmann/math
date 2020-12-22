#include <iostream>

#include <units/length.hpp>
#include <units/inch.hpp>
#include <units/meter.hpp>
#include <units/quantity.hpp>

#include <matrix.hpp>

#include <cxxutility/debugstream.h>

#include "test.hpp"

using namespace nd::math;
using namespace nd::math::units;
using namespace nd::math::units::literals;

int main(int, char **)
{
	{
		Quantity meter = 0.5_m;
		Quantity foo0 = quantity_cast<Inch<double>>(meter);
		foo0 = quantity_cast<Inch<double>>(2_m);
		Quantity a = 2.0_mm;
		std::cout << a << "\n";
		a = quantity_cast<Millimeter<double>>(2.0_m);
		std::cout << quantity_cast<Millimeter<double>>(2.0_m) << "\n";
		std::cout << a << "\n";
		cxxdebug << (typename decltype (2.0_mm)::length_dimension_type){};
		cxxdebug << (typename decltype (quantity_cast<Millimeter<double>>(2.0_m))::length_dimension_type){};
		cxxinfo << "pow";
		cxxdebug << RatioPow<Ratio<1, 1000>, -3>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, -3>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, -2>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, -2>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, -1>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, -1>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, 0>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, 0>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, 1>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, 1>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, 2>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, 2>{};
		cxxdebug << RatioPow<Ratio<1, 1000>, 3>{};
		cxxdebug << RatioPow<Ratio<1000, 1>, 3>{};
		cxxinfo << "mul";
		cxxdebug << RatioMul<Ratio<1, 1000>, Ratio<1, 1000>>{};
	}

	{
		Quantity<Matrix4x4_f> q;
		cxxdebug << q;
	}
}
