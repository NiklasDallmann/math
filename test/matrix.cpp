#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <matrix.hpp>

#include "test.hpp"

using namespace nd::math;

constexpr std::size_t matrixOrder = 512u;
using MatrixType = Matrix<float, matrixOrder, matrixOrder>;

int main(int, char **)
{
	{
		constexpr std::size_t				iterations	= 5u;
		const std::unique_ptr<MatrixType>	matrix0		= std::make_unique<MatrixType>(traits::initialization::zero);
		const std::unique_ptr<MatrixType>	matrix1		= std::make_unique<MatrixType>(traits::initialization::identity);
		std::unique_ptr<MatrixType>			matrix2		= std::make_unique<MatrixType>(traits::initialization::zero);

		std::chrono::duration<double> actualDuration;
		benchmark(iterations, actualDuration, [&matrix0, &matrix1, &matrix2]()
		{
			mul(*matrix2, *matrix0, *matrix1);
		});

		const double seconds	= std::chrono::duration_cast<std::chrono::milliseconds>(actualDuration).count() / 1000.0;
		const double flop		= std::pow(double(matrixOrder), 3.0);
		const double gflops		= (double(iterations) / seconds * flop / 1.0E9);

		std::cout << iterations << " runs performed " << std::to_string(seconds) << " s " << std::to_string(gflops) << " GFLOPS\n";
	}

	{
		const Matrix3x3_f matrix{traits::initialization::identity};

		std::cout << static_cast<Matrix4x4_f>(matrix) << "\n";
	}

	{
		const Vector3_f	v{traits::initialization::zero};
		const float		s = 1.0f;
		std::cout << Vector4_f{s, v} << "\n";

		std::cout << ColumnVector4_f{s, v.transposed()} << "\n";
	}

	{
		const Matrix4x4_f m = {{1.0f, 0.0f, 0.0f, 1.0f,
								1.0f, 0.0f, 0.0f, 1.0f,
								1.0f, 0.0f, 0.0f, 1.0f,
								1.0f, 0.0f, 0.0f, 1.0f}};

		std::cout << m << "\n";
	}

	return EXIT_SUCCESS;
}
