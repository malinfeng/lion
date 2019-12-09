#pragma once
#include "Event.h"
#include <memory>

namespace lion
{
	class TcpConnection;
	class NetAddr;
	class TcpConnectionEvent :
		public Event
	{
	public:
		TcpConnectionEvent(EventLoop *loop, int fd, NetAddr *cliaddr);
		~TcpConnectionEvent();

		std::unique_ptr<TcpConnection> _tcpconnet;

	};
}



