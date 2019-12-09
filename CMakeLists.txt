cmake_minimum_required(VERSION 3.15)
project(MyCloud)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        MyCloud

        Buffer.h Buffer.cpp
        config.h
        Epoll.h Epoll.cpp
        Event.h Event.cpp
        EventFactory.h EventFactory.cpp
        EventLoop.h EventLoop.cpp
        ListenEvent.h ListenEvent.cpp
        MainEventLoop.h MainEventLoop.cpp
        NetAddr.h NetAddr.cpp
        noncopyable.h

        Server.h Server.cpp
        SocketOps.h SocketOps.cpp
        SubEventLoopThread.h SubEventLoopThread.cpp
        SubEventLoopPool.h SubEventLoopPool.cpp
        TcpConnection.h TcpConnection.cpp
        TcpConnectionEvent.h TcpConnectionEvent.cpp
        TimerEvent.h TimerEvent.cpp
        TimesTamp.h TimesTamp.cpp
        TimerQueue.h TimerQueue.cpp
        WakeupEvent.h WakeupEvent.cpp

        main.cpp
        TsetEventFactory.h
        )

find_package(Threads REQUIRED)
target_link_libraries(MyCloud Threads::Threads)