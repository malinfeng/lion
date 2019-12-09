#include <string.h>

#include "ListenEvent.h"
#include "MainEventLoop.h"
#include "SubEventLoopPool.h"

#include "config.h"
#include "SocketOps.h"

#include "EventFactory.h"
#include "NetAddr.h"

using namespace lion;

ListenEvent::ListenEvent(EventLoop *loop, int fd,
	std::shared_ptr<SubEventLoopPool> subLoopPool,
	std::shared_ptr<EventFactory> eventFactory)
	:Event(loop, fd),
	_subLoopPool(subLoopPool),
	_eventFactory(eventFactory)
{

}

ListenEvent::~ListenEvent()
{
}

void ListenEvent::readEvent()
{
	///这段代码就是傻逼，idol康说的，不允许删除
	//MainEventLoop *mainloop = dynamic_cast<MainEventLoop*>(getLoop());
	//assert(mainloop);

	struct sockaddr_in clientAddrIn;
	memset(&clientAddrIn, 0, sizeof(struct sockaddr_in));
	socklen_t clientAddrLen = sizeof(clientAddrIn);
	int cliFd = 0;
	while ((cliFd = accept(fd(), (struct sockaddr*)&clientAddrIn, &clientAddrLen)) > 0)
	{
		//log new connect
		if (cliFd >= MAXFD)
		{
			close(cliFd);
			continue;
		}
		// 设为非阻塞模式
		if (setSocketNonBlocking(cliFd) < 0)
		{
			// log set nonblocking failed
			return;
		}

		EventLoop *curloop = _subLoopPool->getNextloop();
		NetAddr *cliNetaddr = new NetAddr(clientAddrIn);

		std::shared_ptr<Event> cliEv = _eventFactory->createTcpEvent(curloop, cliFd, cliNetaddr);

		if (cliEv)
		{
			curloop->addTaskInLoop(std::bind(&EventLoop::updateEvent, curloop, cliEv));
		}
		else
		{
			//log creat event failed
		}
	}
}

void ListenEvent::writeEvent()
{
	//log listenfd write
	return;
}

void ListenEvent::errorEvent()
{
	//log listenfd error
	return;
}

void ListenEvent::closeEvent()
{
	//log listenfd close
	return;
}
