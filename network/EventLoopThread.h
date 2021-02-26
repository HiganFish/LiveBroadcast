//
// Created by rjd67 on 2021/2/8.
//

#ifndef LIVEBROADCASTSERVER_EVENTLOOPTHREAD_H
#define LIVEBROADCASTSERVER_EVENTLOOPTHREAD_H

#include "network/EventLoop.h"
#include "thread/Thread.h"
#include "thread/Condition.h"

class EventLoopThread
{
public:
	explicit EventLoopThread(const std::string& name);
	~EventLoopThread();

	EventLoop* Start();

	EventLoop* GetEventLoop();

private:
	std::string name_;
	EventLoop* loop_;
	Thread thread_;

	Mutex mutex_;
	Condition cond_;

	void ThreadFunc();
};


#endif //LIVEBROADCASTSERVER_EVENTLOOPTHREAD_H
