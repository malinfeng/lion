#include "MainEventLoop.h"
#include "Event.h"
#include "SocketOps.h"
#include "ListenEvent.h"

#include <iostream>

using namespace lion;

MainEventLoop::MainEventLoop(std::thread::id threadId, int port,
	std::shared_ptr<SubEventLoopPool> eventLoopPool, std::shared_ptr<EventFactory> eventFactory)
	:
	EventLoop(threadId),
	_eventLoopPool(eventLoopPool),
	_eventFactory(eventFactory),
	_listenFd(socket_bind_listen(port)),
	_listenEvent(new ListenEvent(this, _listenFd, _eventLoopPool, _eventFactory)),
	_port(port)
{
    _listenEvent->enableReading();
	updateEvent(_listenEvent);

	runEvery(1000 * 1000, std::bind([](){
		std::cout << "定时任务测试" << std::endl;
	}));
}


MainEventLoop::~MainEventLoop()
{
	_eventLoopPool.reset();
	_listenEvent.reset();
}

