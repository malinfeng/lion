
#include "SubEventLoopThread.h"

#include "EventLoop.h"

using namespace lion;

SubEventLoopThread::SubEventLoopThread()
:   _loop(NULL)
{
    //creat sub thread and start loop
	_thread = new std::thread(&SubEventLoopThread::threadFunc, this);
}

SubEventLoopThread::~SubEventLoopThread()
{
    if (_loop != NULL)
    {
        _loop->quitLoop();
        _thread->join();
    }
}

EventLoop* SubEventLoopThread::getLoop()
{
    {
        std::unique_lock <std::mutex> finishLck(_finishMutex);
        _finishCond.wait(finishLck, [this](){ return _loop != NULL; });
    }

    return _loop;
}

void SubEventLoopThread::threadFunc()
{
    //record cur thread id (cur thread is sub thread)
    _loop = new EventLoop(std::this_thread::get_id());
    _finishCond.notify_one();
    
	_loop->startLoop();

    _loop = NULL;
}