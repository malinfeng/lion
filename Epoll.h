#pragma once

#include <vector>
#include <sys/epoll.h>
#include <memory>


#include "config.h"

namespace lion
{
	class Event;

	typedef std::shared_ptr<Event> Sp_Event;

	class Epoll
	{
	public:
		Epoll();
		~Epoll();
		void addEvent(Sp_Event newEvent);
		void modEvent(Sp_Event newEvent);
		void delEvent(Sp_Event newEvent);

		std::vector<Sp_Event> poll();

	private:
		int _epollfd;
		std::vector<epoll_event> _readyEvents;

		///fd与Channel的映射
		///此处Channel被释放，则永远被释放
		std::shared_ptr<Event> _fd_EventMap[MAXFD];


	};
}



