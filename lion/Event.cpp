#include <assert.h>

#include "Event.h"
#include "SocketOps.h"

using namespace lion;

const int Event::kReadEvent = EPOLLIN | EPOLLPRI;
const int Event::kWriteEvent = EPOLLOUT;



Event::Event(EventLoop* loop, int fd)
	:
	_loop(loop),
	_fd(fd),
	_events(0),
	_revents(0),
	_added(false)
{
	assert(fd < MAXFD);
	if (USE_ET_MOD)
	{
		_events |= EPOLLET;
		assert(setSocketNonBlocking(_fd) == 0);
	}
}


Event::~Event()
{
	close(_fd);
}


void Event::handleEvents()
{
	if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
	{
		closeEvent();
	}
	if (_revents & EPOLLERR)
	{
		errorEvent();
	}
	if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		readEvent();
	}
	if (_revents & EPOLLOUT)
	{
		writeEvent();
	}
}
