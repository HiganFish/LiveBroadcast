#ifndef NETWORK_EVENTLOOP_H
#define NETWORK_EVENTLOOP_H

#include "network/Callback.h"
#include "thread/Mutex.h"

class MultiplexingBase;
class Channel;
class EventLoop
{
public:
	typedef std::function<void()> EventLoopFunction;

	EventLoop();
	~EventLoop();

	void Loop();

	void Stop();

	void Update(Channel* channel);

	void RunInLoop(const EventLoopFunction& function);

	/**
	 * 确认当前线程是EventLoop所属的线程
	 * @return true 是其所属 false 不是其所属
	 */
	bool IsInLoopThread() const;

	EventLoop(const EventLoop& loop) = delete;
	EventLoop& operator=(const EventLoop& loop) = delete;
private:

	MultiplexingBase* multiplexing_base_;

	ChannelVector active_channels_;
	std::vector<EventLoopFunction> pending_func_;

	bool looping_;

	int thread_tid_;

	Mutex pending_func_mutex_;

	void HandleActiveChannel();
	void HandlePendingFunc();
};

#endif // NETWORK_EVENTLOOP_H