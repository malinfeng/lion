#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include "EventFactory.h"
#include "Event.h"
#include "TcpConnection.h"
#include "EventLoop.h"

namespace lion
{
	class TestEvent :public Event
	{
	public:
		TestEvent(EventLoop *loop, int fd)
			:
			Event(loop, fd),
			_pconnection(new TcpConnection(fd))
		{
		}
		~TestEvent()
		{}
	private:

		void readEvent();
		void writeEvent();
		void errorEvent();
		void closeEvent();
	private:
		std::shared_ptr<TcpConnection> _pconnection;
	};


	class TsetEventFactory :
		public EventFactory
	{
	public:
		TsetEventFactory()
		{}
		~TsetEventFactory()
		{}

		std::shared_ptr<Event> cerateEvent(EventLoop *loop, int fd)
		{
			std::shared_ptr<Event> ret = std::make_shared<TestEvent>(loop, fd);
			ret->enableReading();
			return ret;
		}
	};
}


void lion::TestEvent::readEvent() 
{
	if (_pconnection->readFd() > 0)
	{
		std::string hello = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
		_pconnection->appendMsg(hello);

		if (_pconnection->sendMsg() < hello.size())
		{
			enableWriting();
		}
		else
		{
			EventLoop *thisloop = getLoop();
			thisloop->addTaskInLoop(std::bind(&EventLoop::deleteEvent, thisloop, shared_from_this()));
		}
	}
	else
	{
		std::cout << this->fd() << "连接出错" << std::endl;
		EventLoop *thisloop = getLoop();
		thisloop->addTaskInLoop(std::bind(&EventLoop::deleteEvent, thisloop, shared_from_this()));
	}
}

void lion::TestEvent::writeEvent() 
{
	if (_pconnection->sendMsg())
	{

	}
	EventLoop *thisloop = getLoop();
	thisloop->addTaskInLoop(std::bind(&EventLoop::deleteEvent, thisloop, shared_from_this()));
}
void lion::TestEvent::errorEvent() 
{
	std::cout << "连接业务，error, fd  "<< this->fd() << std::endl;
}
void lion::TestEvent::closeEvent() 
{
	std::cout << "连接业务，close, fd  "<< this->fd() << std::endl;
}

