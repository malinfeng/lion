#include <sstream>

#include "EventLoop.h"
#include "Epoll.h"
#include "WakeupEvent.h"
#include "SocketOps.h"
#include "TimerQueue.h"
#include "TimerEvent.h"

using namespace lion;

uint64_t lion::GetthreadID(std::thread::id threadId)
{
	std::stringstream ss;
	ss << threadId;
	return std::stoull(ss.str());
}

EventLoop::EventLoop(std::thread::id threadId)
	:
	_quit(false),
	_looping(false),
	_doingWaitTasks(false),
	_doingEpollEvents(false),
	_threadId(GetthreadID(threadId)),
	_wakeupFd(createEventFd()),
	_timerFd(createTimerfd()),
	_epoll(new Epoll()),
	_timerCbs(new TimerQueue(_timerFd)),
	_wakeupEvent(new WakeupEvent(this, _wakeupFd)),
	_timerEvent(new TimerEvent(this, _timerFd))
{
	_wakeupEvent->enableReading();
	updateEvent(_wakeupEvent);
	_timerEvent->enableReading();
	updateEvent(_timerEvent);
}


EventLoop::~EventLoop()
{
	deleteEvent(_wakeupEvent);
	_wakeupEvent.reset();
}

void EventLoop::startLoop()
{
	assert(!_looping);
	assertInLoopThread();
	_looping = true;
	_quit = false; 
	//log eventloop start

	std::vector<Sp_Event> ret;

	while (!_quit)
	{
		ret.clear();
		ret.clear();
		ret = _epoll->poll();

		_doingEpollEvents = true;
		for (auto &it : ret)
		{
			it->handleEvents();
		}
		_doingEpollEvents = false;
		doWaitTasks();//处理任务
	}

	//log eventloop quit
	_looping = false;
}

void EventLoop::doWaitTasks()
{
	assertInLoopThread();
	std::vector<Task> tasks;
	_doingWaitTasks = true;

	{
		std::lock_guard<std::mutex> lock(_mutex);
		tasks.swap(_waitTasks);
	}

	for (size_t i = 0; i < tasks.size(); ++i)
	{
		tasks[i]();
	}
	_doingWaitTasks = false;
}

void EventLoop::quitLoop()
{
	_quit = true;
	if (!isInLoopThread())
	{
		//阻塞在epoll
		wakeup();
	}
}

void EventLoop::wakeup()
{
	//其他线程往wakeupfd里写，唤醒wait的线程
	uint64_t one = 1;
	write(_wakeupFd, (char*)(&one), sizeof one);
}

void EventLoop::addTaskInLoop(Task&& task)
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_waitTasks.emplace_back(std::move(task));
	}
	//有可能正在处理任务队列，任然需要唤醒
	if (!isInLoopThread() || _doingWaitTasks)
	{
		wakeup();
	}
}

void EventLoop::updateEvent(Sp_Event event)
{
	assertInLoopThread();
	if (event->isAdded())
	{
		_epoll->modEvent(event);
	}
	else
	{
		_epoll->addEvent(event);
	}
}

void EventLoop::deleteEvent(Sp_Event event)
{
	assertInLoopThread();
	_epoll->delEvent(event);
}

void EventLoop::runAfter(int us, Task task)
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_timerCbs->addTask(us, task);
	}
}

void EventLoop::runEvery(int us, Task task)
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_timerCbs->addRepeatTask(us, task);
	}
}

void EventLoop::doTimerTasks()
{
	assertInLoopThread();
	_timerCbs->handleEvent();
}