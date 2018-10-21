#pragma once

#include <chrono>

class Timer
{
public:
	void Start()
	{
		start = std::chrono::steady_clock::now();
	}
	double Mark()
	{
		const auto now = std::chrono::steady_clock::now();
		const std::chrono::duration<double> time = now - start;

		return time.count();
	}

private:
	std::chrono::steady_clock::time_point start;
};