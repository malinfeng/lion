#pragma once
#include <sys/epoll.h>
#include <memory>

#include "config.h"


namespace lion
{
	class EventLoop;

	class Event : public std::enable_shared_from_this<Event>
	{
	public:
		Event(EventLoop* loop, int fd);
		virtual ~Event();

		int fd() const { return _fd; }
		void handleEvents();

		void enableReading() { _events |= kReadEvent; }
		void disableReading() { _events &= ~kReadEvent; }
		void enableWriting() { _events |= kWriteEvent;  }
		void disableWriting() { _events &= ~kWriteEvent; }

		bool isWriting() const { return _events & kWriteEvent; }
		bool isReading() const { return _events & kReadEvent; }

		bool isAdded() const { return _added; }
		void setAdded() { _added = true; }
		void takeoff() { _added = false; }

		void disableAll() 
		{
			_events = 0;
			if (USE_ET_MOD)
			{
				_events &= EPOLLET;
			}
		}

		//poller use
		void setRevents(int ev) { _revents = ev; }

		int getEvents() const { return _events; }
		EventLoop * getLoop()const { return _loop; }
	private:
		virtual void readEvent() = 0;
		virtual void writeEvent() = 0;
		virtual void errorEvent() = 0;
		virtual void closeEvent() = 0;

	private:
		//事件是与EventLoop强相关的，持有该事件的Loop
		EventLoop* _loop;
		const int _fd;
		
		int _events;
		int _revents;

		bool _added;

		static const int kReadEvent;
		static const int kWriteEvent;
	};

}

