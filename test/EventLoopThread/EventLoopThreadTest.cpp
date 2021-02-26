//
// Created by rjd67 on 2021/2/23.
//
#include <unistd.h>
#include "network/EventLoopThreadPool.h"

void Func()
{
	printf("Func\n");
}

int main()
{
	EventLoop loop;

	EventLoopThreadPool pool(&loop, "UserMapper pool");
	pool.SetThreadNum(4);

	pool.Start();

	EventLoop* loop1 = pool.GetNextLoop();
	EventLoop* loop2 = pool.GetNextLoop();

	loop1->RunInLoop(Func);
	loop2->RunInLoop(Func);

	sleep(2);

	printf("Main\n");

	return 0;
}