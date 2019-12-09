#pragma once
#include <functional>
#include <map>


namespace lion
{
	typedef std::function<void()> Task;

	class TimerTask
	{
	public:
		TimerTask() {}
		TimerTask(Task cb)
			:
			_cb(cb),
			_repeatTime(0)
		{}

		void operator()() { _cb();}

		Task _cb;
		int _repeatTime;
	};

	class TimesTamp;
	class TimerQueue
	{
	public:
		TimerQueue(int timerfd);
		~TimerQueue();

		void handleEvent();

		void addTask(int time_us, Task cb);
		void addRepeatTask(int time_us, Task cb);
	private:

		//更新新的定时时间，us
		void updateTimerTime(long long newtime);
	private:
		int _timerfd;
		long long _earliestTime;
		std::map<TimesTamp, TimerTask> _timerEventcbs;
	};
}


