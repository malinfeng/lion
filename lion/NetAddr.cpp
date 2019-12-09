#include "NetAddr.h"

using namespace lion;

NetAddr::NetAddr(const struct sockaddr_in addr)
	:
	_addr(addr)
{
}

NetAddr::NetAddr(std::string ip, uint16_t port)
{
	_addr.sin_family = AF_INET;
	_addr.sin_port = htobe16(port);;
	if (::inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr) <= 0)
	{
		//log
	}
}

NetAddr::~NetAddr()
{
}

std::string NetAddr::getIp() const
{
	char buf[64] = "";
	::inet_ntop(AF_INET, &_addr.sin_addr, buf, static_cast<socklen_t>(sizeof(buf)));
	return buf;
}

