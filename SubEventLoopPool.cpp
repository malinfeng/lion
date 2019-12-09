#include <assert.h>


#include "SubEventLoopPool.h"
#include "SubEventLoopThread.h"


using namespace lion;

SubEventLoopPool::SubEventLoopPool(int threadNum)
	:
	_threadNum(threadNum),
	_started(false),
	_nextThreadPos(0)
{
	assert(threadNum > 0);
	_started = true;
	for (int i = 0; i < _threadNum; ++i)
	{
		std::shared_ptr<SubEventLoopThread> t(new SubEventLoopThread());
		_threads.push_back(t);
	}
}


SubEventLoopPool::~SubEventLoopPool()
{
}

void SubEventLoopPool::start()
{
	for (int i = 0; i < _threadNum; ++i)
	{
		_loops.push_back(_threads[i]->getLoop());
	}
	assert(_loops.size() == _threads.size());
}

EventLoop *SubEventLoopPool::getNextloop()
{
	//ensure thread started
	assert(_started);
	EventLoop *ploop = _loops[_nextThreadPos];
	assert(ploop);

	_nextThreadPos = (_nextThreadPos + 1) % _threadNum;

	return ploop;
}