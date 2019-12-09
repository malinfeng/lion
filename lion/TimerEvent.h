#pragma once
#include "Event.h"

namespace lion
{
	int createTimerfd();

	class TimerEvent :
		public Event
	{
	public:
		TimerEvent(EventLoop *loop, int fd);
		~TimerEvent();
	private:
		virtual void readEvent();
		virtual void writeEvent();
		virtual void errorEvent();
		virtual void closeEvent();
	};
}

