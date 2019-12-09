#pragma once
#include <vector>
#include <thread>

#include "noncopyable.h"

namespace lion
{
	class EventLoop;
	class SubEventLoopThread;

	class SubEventLoopPool :public noncopyable
	{
	public:
		SubEventLoopPool(int threadNum);
		~SubEventLoopPool();
		void start();

		EventLoop *getNextloop();

	private:
		const int _threadNum;
		bool _started;
		int _nextThreadPos;

		//subloop的生命期依托于此
		std::vector<EventLoop *> _loops;
		std::vector<std::shared_ptr<SubEventLoopThread>> _threads;
	};
}


