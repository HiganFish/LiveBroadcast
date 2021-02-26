//
// Created by rjd67 on 2021/2/8.
//

#ifndef LIVEBROADCASTSERVER_EVENTLOOPTHREADPOOL_H
#define LIVEBROADCASTSERVER_EVENTLOOPTHREADPOOL_H

#include "network/EventLoopThread.h"

class EventLoopThreadPool
{
public:

	/**
	 * 创建EventLoop线程池
	 * @param loop 默认EventLoop循环
	 * @param name 名称
	 */
	EventLoopThreadPool(EventLoop* loop, const std::string& name);
	~EventLoopThreadPool();

	/**
	 * 设置线程数量
	 * @param thread_num
	 */
	void SetThreadNum(int thread_num);

	/**
	 * 启动线程池
	 */
	void Start();

	/**
	 * 轮次取出个个eventloop 当线程数量为0时 返回默认线程
	 * @return eventloop
	 */
	EventLoop* GetNextLoop();
private:

	EventLoop* base_loop_;

	std::string name_;

	int next_thread_id_;
	int thread_num_;

	std::vector<std::unique_ptr<EventLoopThread>> threads_;
	std::vector<EventLoop*> loops_;
};


#endif //LIVEBROADCASTSERVER_EVENTLOOPTHREADPOOL_H
