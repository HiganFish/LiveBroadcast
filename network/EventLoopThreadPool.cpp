//
// Created by rjd67 on 2021/2/8.
//

#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* base_loop, const std::string& name):
	base_loop_(base_loop),
	name_(name),
	next_thread_id_(0),
	thread_num_(0)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

void EventLoopThreadPool::SetThreadNum(int thread_num)
{
	thread_num_ = thread_num;
}
void EventLoopThreadPool::Start()
{
	char buf[32];
	for (int i = 0; i < thread_num_; ++i)
	{
		snprintf(buf, sizeof buf, "%s#%d", name_.c_str(), i);
		EventLoopThread* thread = new EventLoopThread(buf);
		threads_.push_back(std::unique_ptr<EventLoopThread>(thread));
		loops_.push_back(thread->Start());
	}
}

EventLoop* EventLoopThreadPool::GetNextLoop()
{
	if (thread_num_ == 0)
	{
		return base_loop_;
	}

	EventLoop* ret = loops_[next_thread_id_];
	next_thread_id_ = (next_thread_id_ + 1) % thread_num_;
	return ret;
}