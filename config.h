#pragma once
//参数需要合法性检测

//fd的触发模式，默认使用ET非阻塞IO模式
#define USE_ET_MOD 1

//fd的上限值
#define MAXFD 65535

//epoll wait 的ready队列上限
#define EPOLL_READY_MAX 4096

//epoll wait 的wait时间 ms
#define EPOLLWAIT_TIME 1000
