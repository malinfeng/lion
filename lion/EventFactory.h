#pragma once
#include <memory>

namespace lion
{
	class Event;
	class EventLoop;
	class NetAddr;

	class EventFactory
	{
	public:
		EventFactory();
		virtual ~EventFactory();

		virtual std::shared_ptr<Event> createTcpEvent(EventLoop *loop, int fd, NetAddr *cliaddr) = 0;

	};
}



