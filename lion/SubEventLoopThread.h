#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "noncopyable.h"

namespace lion
{
	class EventLoop;
	class Event;
	typedef std::shared_ptr<Event> Sp_Event;

	//将子loop与线程的简单封装
	class SubEventLoopThread : noncopyable
	{
	public:
		SubEventLoopThread();
		~SubEventLoopThread();
		EventLoop* getLoop();

	private:
		void threadFunc();

	private:
		//EventLoop的生命在此
		EventLoop *_loop;
		std::thread *_thread;

		std::mutex _finishMutex;
		std::condition_variable _finishCond;
	};
}

