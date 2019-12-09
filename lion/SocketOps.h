#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/eventfd.h>

namespace lion
{
	int socket_bind_listen(int port);

	int createEventFd();

	//关闭Nagel算法
	void setSocketNodelay(int fd);
	//非阻塞
	int setSocketNonBlocking(int fd);

	ssize_t read(int sockfd, void *buf, size_t count);
	ssize_t write(int sockfd, const void *buf, size_t count);
}