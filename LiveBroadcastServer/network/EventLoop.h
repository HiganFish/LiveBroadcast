#ifndef NETWORK_EVENTLOOP_H
#define NETWORK_EVENTLOOP_H

#include "network/Callback.h"

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

	EventLoop(const EventLoop& loop) = delete;
	EventLoop& operator=(const EventLoop& loop) = delete;
private:

	MultiplexingBase* multiplexing_base_;

	ChannelVector active_channels_;
	std::vector<EventLoopFunction> pending_func_;

	bool looping_;

	void HandleActiveChannel();
	void HandlePendingFunc();
};

#endif // NETWORK_EVENTLOOP_H