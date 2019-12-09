#pragma once
#include "EventLoop.h"
#include <memory>

namespace lion
{
	class SubEventLoopPool;
	class EventFactory;

	class MainEventLoop :
		public EventLoop
	{
	public:
		MainEventLoop(std::thread::id threadId, int port, 
			std::shared_ptr<SubEventLoopPool> eventLoopPool, std::shared_ptr<EventFactory> eventFactory);
		~MainEventLoop();

	private:
		std::shared_ptr<SubEventLoopPool> _eventLoopPool;
		std::shared_ptr<EventFactory> _eventFactory;

		int _listenFd;
		std::shared_ptr<Event> _listenEvent;
		int _port;
	};

}


