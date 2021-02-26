//
// Created by rjd67 on 2021/2/8.
//

#include "EventLoopThread.h"

EventLoopThread::EventLoopThread(const std::string& name):
	name_(name),
	loop_(nullptr),
	thread_([this] { ThreadFunc(); }, name),
	mutex_(),
	cond_(mutex_)
{

}

EventLoopThread::~EventLoopThread()
{
	if (loop_ != nullptr)
	{
		loop_->Stop();
		thread_.Join();
	}
}

EventLoop* EventLoopThread::Start()
{
	thread_.Start();

	EventLoop* loop = nullptr;
	{
		MutexLockGuard guard(mutex_);
		while (loop_ == nullptr) // 防止虚假唤醒
		{
			cond_.Wait();
		}
		loop = loop_; // TODO 为什么不直接在 创建成员变量 EventLoop loop_; ??
		// TODO 为了等线程创建完毕后 才返回? 否则这个函数只有第一句 可能线程未创建完毕就返回了?
	}
	return loop;
}

void EventLoopThread::ThreadFunc()
{
	EventLoop loop;
	{
		MutexLockGuard guard(mutex_);
		loop_ = &loop;
		cond_.WakeUpOne();
	}

	loop.Loop();
	MutexLockGuard lock(mutex_);
	loop_ = nullptr;
}

EventLoop* EventLoopThread::GetEventLoop()
{
	return loop_;
}
