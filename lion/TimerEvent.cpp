#include "TimerEvent.h"
#include <sys/timerfd.h>

#include "EventLoop.h"
#include "SocketOps.h"

using namespace lion;

int lion::createTimerfd()
{
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
		TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0)
	{
		//log
	}
	return timerfd;
}

TimerEvent::TimerEvent(EventLoop *loop, int fd)
	:
	Event(loop, fd)
{
}


TimerEvent::~TimerEvent()
{
}

void TimerEvent::readEvent()
{
    uint64_t howmany;
    ssize_t n = ::read(fd(), &howmany, sizeof howmany);
    if (n != sizeof howmany)
    {

    }

	getLoop()->doTimerTasks();
}

void TimerEvent::writeEvent()
{
	//log
	return;
}

void TimerEvent::errorEvent()
{
	//log
	return;
}

void TimerEvent::closeEvent()
{
	//log
	return;
}