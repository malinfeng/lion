#include "TcpConnection.h"
#include "Buffer.h"
#include "SocketOps.h"

using namespace lion;

TcpConnection::TcpConnection(int fd)
	:
	_fd(fd),
    _pInBuffer(new Buffer()),
    _pOutBuffer(new Buffer())
{
	_tcpState = connected;
}

TcpConnection::TcpConnection(int fd, NetAddr *pnetaddr)
	:
	_fd(fd),
    _pInBuffer(new Buffer()),
    _pOutBuffer(new Buffer()),
    _tcpNetaddr(pnetaddr)
{
	_tcpState = connected;
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::appendMsg(std::string &msg)
{
	_pOutBuffer->append(&*msg.begin(), msg.size());
}

int TcpConnection::sendMsg()
{
	std::string alldata = _pOutBuffer->getAllString();
	size_t nleft = alldata.size();
	ssize_t nwritten = 0;
	ssize_t writeSum = 0;
	const char *ptr = alldata.c_str();
	while (nleft > 0)
	{
		if ((nwritten = write(_fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0)
			{
				if (errno == EINTR)
				{
					nwritten = 0;
					continue;
				}
				else if (errno == EAGAIN)
				{
					break;
				}
				else//此处逻辑待修改
				{
					//出错
					return -1;
				}
			}
		}
		writeSum += nwritten;
		nleft -= nwritten;
		ptr += nwritten;
	}

	if (writeSum < static_cast<int>(alldata.size()))
	{
		_pOutBuffer->append(ptr, nleft);
	}
	return writeSum;
}

int TcpConnection::readFd()
{
	ssize_t nread = 0;
	ssize_t readSum = 0;
	while (true)
	{
		char buff[BUFFER_MAX];
		if ((nread = read(_fd, buff, BUFFER_MAX)) < 0)
		{
			if (errno == EINTR)
				continue;
			else if (errno == EAGAIN)
			{
				return readSum;
			}
			else
			{
				//出错
				return -1;
			}
		}
		else if (nread == 0)
		{
			break;
		}
		readSum += nread;
		_pInBuffer->append(buff, nread);
	}
	return readSum;
}
