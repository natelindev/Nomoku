//
//  Timer.hpp
//  Nomoku
//
//

#ifndef Timer_h
#define Timer_h

#include <chrono>
#include <ctime>

class Timer
{
	double time_limit; //second
	std::chrono::time_point<std::chrono::system_clock> start_time;

	void update_current_time();

public:
	Timer();
	~Timer();

	double get_time_limit();
	void set_time_limit(double limit);

	//once start again,the previous start_time would be lost
	void start();
	bool have_time_left();

};
#endif /* Timer_h */
