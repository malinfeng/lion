#pragma once

namespace lion
{
	class EventFactory;
	class MainEventLoop;
	class SubEventLoopPool;

	class Server
	{
	public:
		Server(int port, int subLoopNum, EventFactory *eventFactory);
		~Server();

		void start();

	private:
		//主线程创建
		SubEventLoopPool *_loopPool;

		MainEventLoop *_mainloop;

		EventFactory *_eventFactory;
	};

}


