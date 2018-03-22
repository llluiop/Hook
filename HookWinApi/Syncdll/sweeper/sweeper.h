#ifndef __sweeper_h__
#define __sweeper_h__

#include <Windows.h>
#include <future>
#include <atomic>
#include <thread>

class Sweeper
{
public:
	static Sweeper* Instance()
	{
		static Sweeper instance;
		return &instance;
	}

	bool Start();
	void Stop();

	void SaveAsTriggered();

private:
	Sweeper();

	std::atomic_bool quit_ = false;
	HANDLE sigs_[2]; //0 for save, 1 for copy
	std::thread t_;
};


#endif
