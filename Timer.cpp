//
//  Timer.cpp
//  Nomoku
//
//  This class is used for timing
//

#include "Timer.h"

Timer::Timer() :time_limit(5){}

void Timer::start()
{
	start_time = std::chrono::system_clock::now();
}

bool Timer::have_time_left()
{
	std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start_time;
	return (elapsed_seconds.count()<time_limit);
}

double Timer::get_time_limit()
{
	return time_limit;
}

void Timer::set_time_limit(double limit)
{
	time_limit = limit;
}
