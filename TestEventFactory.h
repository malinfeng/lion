#pragma once
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include "EventFactory.h"
#include "Event.h"
#include "EventLoop.h"
#include "TcpConnectionEvent.h"
#include "TcpConnection.h"

namespace lion
{
	class TestEvent :public TcpConnectionEvent
	{
	public:
		TestEvent(EventLoop *loop, int fd, NetAddr *addr)
			:
			TcpConnectionEvent(loop, fd, addr)
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
	};


	class TestEventFactory :
		public EventFactory
	{
	public:
		TestEventFactory()
		{}
		~TestEventFactory()
		{}

		std::shared_ptr<Event> createTcpEvent(EventLoop *loop, int fd, NetAddr *cliaddr) override
		{
			std::shared_ptr<Event> ret = std::make_shared<TestEvent>(loop, fd, cliaddr);
			ret->enableReading();
			return ret;
		}
	};
}


void lion::TestEvent::readEvent() 
{
	if (_tcpconnet->readFd() > 0)
	{
		std::string hello = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\nHello World";
		_tcpconnet->appendMsg(hello);

		if (_tcpconnet->sendMsg() < hello.size())
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
	if (_tcpconnet->sendMsg())
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

