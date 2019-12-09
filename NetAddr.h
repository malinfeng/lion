#pragma once
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace lion
{
	//对 sockaddr_in 的简单封装
	class NetAddr
	{
	public:
		explicit NetAddr(const struct sockaddr_in addr);
		NetAddr(std::string ip, uint16_t port);
		~NetAddr();

		sockaddr_in getSockaddrIn() const { return _addr; }

		uint16_t getPort() const { return _addr.sin_port; }
		std::string getIp() const;

		struct sockaddr* getSockaddr()
		{
			return static_cast<struct sockaddr*>((void*)&_addr);
		}
	private:
		//网络字节序
		sockaddr_in _addr;
	};
}



