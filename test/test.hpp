#include <chrono>
#include <cmath>
#include <cstdlib>

using namespace std::chrono_literals;

template <typename T>
inline void assertEqual(const T &actual, const T &expected)
{
	if (actual != expected)
	{
		std::exit(EXIT_FAILURE);
	}
}

template <typename T>
inline void assertUnequal(const T &actual, const T &expected)
{
	if (actual != expected)
	{
		std::exit(EXIT_FAILURE);
	}
}

template <typename F>
inline std::size_t benchmark(const std::chrono::duration<double> minimumDuration, std::chrono::duration<double> &actualDuration, const F &function)
{
	const std::chrono::time_point	begin		= std::chrono::high_resolution_clock::now();
	std::chrono::time_point			end			= std::chrono::high_resolution_clock::now();
	std::chrono::duration			duration	= end - begin;

	std::size_t runs = 0u;

	do
	{
		end			= std::chrono::high_resolution_clock::now();
		duration	= end - begin;
		runs++;

		function();
	}
	while (duration < minimumDuration);

	actualDuration = duration;

	return runs;
}

template <typename F>
inline void benchmark(const std::size_t iterations, std::chrono::duration<double> &actualDuration, const F &function)
{
	const std::chrono::time_point begin = std::chrono::high_resolution_clock::now();

	for (std::size_t iteration = 0u; iteration < iterations; ++iteration)
	{
		function();
	}

	const std::chrono::time_point end = std::chrono::high_resolution_clock::now();

	actualDuration = end - begin;
}
