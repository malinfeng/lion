#include <thread>
#include <memory>

#include "Server.h"
#include "MainEventLoop.h"
#include "SubEventLoopPool.h"
#include "EventFactory.h"

using namespace lion;

Server::Server(int port, int subLoopNum, EventFactory *eventFactory):
	_loopPool(new SubEventLoopPool(subLoopNum)),
	_mainloop(new MainEventLoop(std::this_thread::get_id(),port, 
		std::shared_ptr<SubEventLoopPool>(_loopPool), std::shared_ptr<EventFactory>(eventFactory))),
	_eventFactory(eventFactory)
{

}


Server::~Server()
{
}

void Server::start()
{
	_loopPool->start();
	_mainloop->startLoop();
}
