#pragma once
#include "Event.h"

namespace lion
{
	class WakeupEvent :public Event
	{
	public:
		WakeupEvent(EventLoop *loop, int fd) :Event(loop, fd)
		{
		}
		
		~WakeupEvent()
		{
		}
		
		virtual void readEvent() override;
		virtual void writeEvent() override;
		virtual void errorEvent() override;
		virtual void closeEvent() override;
	private:
	};
}


