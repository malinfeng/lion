#pragma once
#include "Event.h"
#include <memory>

namespace lion
{
	class SubEventLoopPool;
	class EventFactory;
	class ListenEvent :public Event
	{
	public:
		ListenEvent(EventLoop *loop, int fd,
			std::shared_ptr<SubEventLoopPool> subLoopPool,
			std::shared_ptr<EventFactory> eventFactory);

		~ListenEvent();

	private:
		//listenfd 的read事件 即新连接来到 需要accepte
		virtual void readEvent() override;
		virtual void writeEvent() override;
		virtual void errorEvent() override;
		virtual void closeEvent() override;

	private:
		std::shared_ptr<SubEventLoopPool> _subLoopPool;

		//此处可改为工厂数组，可生产不同逻辑的连接对象
		std::shared_ptr<EventFactory> _eventFactory;
	};
}

