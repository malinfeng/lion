#include "TimesTamp.h"

using namespace lion;

TimesTamp::TimesTamp(int microSecond, bool fromNow)
{
	if (fromNow)
	{
		_usSinceEpoch = now_us() + microSecond;
	}
	else
	{
		_usSinceEpoch = microSecond;
	}
}


TimesTamp::~TimesTamp()
{
}

long long TimesTamp::now_us() const
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	long long seconds = tv.tv_sec;
	return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

long long TimesTamp::overNow_us() const
{
	return _usSinceEpoch - now_us();
}

void TimesTamp::addtime_us(int ms)
{
	_usSinceEpoch += ms;
}