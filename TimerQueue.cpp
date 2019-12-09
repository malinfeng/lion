#include "TimerQueue.h"
#include "TimesTamp.h"
#include <vector>
#include <assert.h>
#include <sys/timerfd.h>

using namespace lion;

TimerQueue::TimerQueue(int timerfd)
	:
	_timerfd(timerfd),
	_earliestTime(-1)
{
}


TimerQueue::~TimerQueue()
{
}

void TimerQueue::handleEvent()
{
	TimesTamp now(0);
	std::map<TimesTamp, TimerTask>::iterator end = _timerEventcbs.lower_bound(now);
	assert(end == _timerEventcbs.end() || now < end->first);
	
	std::vector<TimerTask> repeatTask;

	for (std::map<TimesTamp, TimerTask>::iterator it = _timerEventcbs.begin(); it != end; ++it)
	{
		if (it->second._repeatTime > 0)
		{
			repeatTask.emplace_back(it->second);
		}
		it->second();
	}

	_timerEventcbs.erase(_timerEventcbs.begin(), end);

	for (int i = 0; i < repeatTask.size(); ++i)
	{
        addRepeatTask(repeatTask[i]._repeatTime, std::move(repeatTask[i]._cb));
	}
	if (_timerEventcbs .empty())
	{
		_earliestTime = -1;
	}
	else
	{
		_earliestTime = _timerEventcbs.begin()->first.usSinceEpoch();
		updateTimerTime(_timerEventcbs.begin()->first.overNow_us());
	}
	
}

void TimerQueue::addTask(int time_us, Task cb)
{
	TimesTamp tempTimesTamp(time_us);
	if (_earliestTime < 0 || tempTimesTamp.usSinceEpoch() < _earliestTime)//堆顶时间短，更新定时时间
	{
		_earliestTime = tempTimesTamp.usSinceEpoch();
		updateTimerTime(time_us);
	}
	TimerTask temptask(cb);



	_timerEventcbs[tempTimesTamp] = temptask;
}

void TimerQueue::addRepeatTask(int time_us, Task cb)
{
	TimesTamp tempTimesTamp(time_us);
	if (_earliestTime < 0 || tempTimesTamp.usSinceEpoch() < _earliestTime)//堆顶时间短，更新定时时间
	{
		_earliestTime = tempTimesTamp.usSinceEpoch();
		updateTimerTime(time_us);
	}
	TimerTask temptask(cb);
	temptask._repeatTime = time_us;
	_timerEventcbs[tempTimesTamp] = temptask;
}

void TimerQueue::updateTimerTime(long long newtime_us)
{
	struct itimerspec newValue;
	struct itimerspec oldValue;
	//memset(&newValue, sizeof newValue);
	//memset(&oldValue, sizeof oldValue);

	if (newtime_us < 100)
	{
		newtime_us = 100;
	}
	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(
		newtime_us / TimesTamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(
		(newtime_us % TimesTamp::kMicroSecondsPerSecond) * 1000);

	newValue.it_value = ts;
	int ret = ::timerfd_settime(_timerfd, 0, &newValue, &oldValue);
	if (ret)
	{
		//log
	}
}
