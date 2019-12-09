#include <assert.h>
#include <unistd.h>

#include "Epoll.h"
#include "Event.h"


using namespace lion;

Epoll::Epoll()
	:
	_epollfd(epoll_create1(EPOLL_CLOEXEC)),
	_readyEvents(EPOLL_READY_MAX)
{
	assert(_epollfd >= 0);
}


Epoll::~Epoll()
{
	close(_epollfd);
}

void Epoll::addEvent(Sp_Event newEvent)
{
	int fd = newEvent->fd();
	
	struct epoll_event event;
	event.data.fd = fd;
	event.events = newEvent->getEvents();

	_fd_EventMap[fd] = newEvent;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		//log perror("epoll_add error");
	}
	newEvent->setAdded();
}

void Epoll::modEvent(Sp_Event newEvent)
{
	int fd = newEvent->fd();

	struct epoll_event event;
	event.data.fd = fd;
	event.events = newEvent->getEvents();
	if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event) < 0)
	{
		//log perror("epoll_mod error");
	}
}

void Epoll::delEvent(Sp_Event newEvent)
{
	int fd = newEvent->fd();

	struct epoll_event event;
	event.data.fd = fd;
	event.events = newEvent->getEvents();

	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &event) < 0)
	{
		//log perror("epoll_del error");
	}
	newEvent->takeoff();
	_fd_EventMap[fd].reset();
}

std::vector<Sp_Event> Epoll::poll()
{
	while (true)
	{
		int event_count = epoll_wait(_epollfd, &*_readyEvents.begin(), _readyEvents.size(), EPOLLWAIT_TIME);
		if (event_count < 0)
		{
			//log perror("epoll wait error");
		}

		std::vector<Sp_Event> ret_data;
		for (int i = 0; i < event_count; ++i)
		{
			// 获取有事件产生的描述符
			int fd = _readyEvents[i].data.fd;

			Sp_Event cur_ev = _fd_EventMap[fd];

			if (cur_ev)
			{
				cur_ev->setRevents(_readyEvents[i].events);
				ret_data.push_back(cur_ev);
			}
		}
		if (ret_data.size() > 0)
		{
			return ret_data;
		}
	}
}