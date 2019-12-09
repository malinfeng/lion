#pragma once
#include <memory>
#include <string>
#include "NetAddr.h"

namespace lion
{
	class Buffer;
	class NetAddr;

	enum TcpState
	{
		connected = 0,
		disconnected,
		disconnecting
	};

	//用来描述一个Tcp连接
	class TcpConnection
	{
	public:
		TcpConnection(int fd);
		TcpConnection(int fd, NetAddr *pnetaddr);
		~TcpConnection();
		void appendMsg(std::string &msg);

		//返回发送的字节数
		int sendMsg();
		//返回读取的字节数
		int readFd();

		std::shared_ptr<Buffer> getInBuffer() { return _pInBuffer; }
		std::shared_ptr<Buffer> getOutBuffer() { return _pOutBuffer; }

		std::string getIp() { return _tcpNetaddr->getIp(); }
		uint16_t getPort() { return _tcpNetaddr->getPort(); }

	private:
		static const int BUFFER_MAX = 4096;

		int _fd;
		std::shared_ptr<Buffer> _pInBuffer;
		std::shared_ptr<Buffer> _pOutBuffer;

		std::unique_ptr<NetAddr> _tcpNetaddr;

		TcpState _tcpState;


	};
}



