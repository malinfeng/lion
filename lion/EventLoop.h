#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>
#include <thread>
#include <assert.h>

#include "noncopyable.h"

namespace lion
{
	uint64_t GetthreadID(std::thread::id threadId);
	
	class Epoll;
	class Event;
	class TimerQueue;
	typedef std::shared_ptr<Event> Sp_Event;
	typedef std::function<void()> Task;
	
	//Loop对象由一个线程创建，并且只能由该线程startLoop
	class EventLoop : noncopyable
	{
	public:
		EventLoop(std::thread::id threadId);
		virtual ~EventLoop();

		void startLoop();

		void quitLoop();

		void addTaskInLoop(Task &&task);

		void wakeup();

		void updateEvent(Sp_Event event);

		void deleteEvent(Sp_Event event);

		void runAfter(int us, Task task);
		void runEvery(int us, Task task);
		void doTimerTasks();

	private:
		void doWaitTasks();

		bool isInLoopThread()
		{
			return _threadId == GetthreadID(std::this_thread::get_id());
		}
		void assertInLoopThread()
		{
			assert(isInLoopThread());
		}

	private:
		std::atomic<bool> _quit;//多线程原子
		bool _looping;
		bool _doingWaitTasks;
		bool _doingEpollEvents;
		const uint64_t _threadId;

		int _wakeupFd;
		int _timerFd;
		std::unique_ptr<Epoll> _epoll;
		std::unique_ptr<TimerQueue> _timerCbs;
		
		std::shared_ptr<Event> _wakeupEvent;
		std::shared_ptr<Event> _timerEvent;

		std::vector<Task> _waitTasks;

		mutable std::mutex _mutex;
	};
}


