#include "TcpConnectionEvent.h"
#include "TcpConnection.h"

using namespace lion;

TcpConnectionEvent::TcpConnectionEvent(EventLoop *loop, int fd, NetAddr *cliaddr)
	:
	Event(loop, fd),
	_tcpconnet(new TcpConnection(fd, cliaddr))
{
}


TcpConnectionEvent::~TcpConnectionEvent()
{
}
