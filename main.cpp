// LiveBroadcastServer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <csignal>
#include "live/RtmpPushServer.h"
#include "live/HttpPullServer.h"

#include "utils/Logger.h"

#ifdef ENABLE_MAPPER
#include "mapper/UserMapper.h"
// UserMapper user_mapper_;

bool OnAuthenticate(const std::string& user, const std::string& passwd)
{
	return true;
//	if (user_mapper_.GetPasswdByUser(user) == passwd)
//	{
//		return true;
//	}
//	else
//	{
//		LOG_WARN << "user: " << user << ", use wrong passwd: " << passwd;
//		return false;
//	}
}
#endif

EventLoop loop;
void StopServer(int)
{
	loop.Stop();
}

void InitSignal()
{
	struct sigaction sa{};
	sa.sa_handler = StopServer;
	sigfillset(&sa.sa_mask);
	sigaction(SIGINT, &sa , nullptr);
	signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("wrong number of parameters\r\n");
		exit(-1);
	}
#ifdef ENABLE_MAPPER
//	if (!user_mapper_.Initialize(
//			"127.0.0.1", "lsmg", "123456789", "live"))
//	{
//		exit(-1);
//	}
#endif

	InitSignal();

	InetAddress rtmp_server_address(argv[1], true);
	InetAddress client_server_address(argv[2], true);

	RtmpPushServer rtmp_push_server(&loop, "rtmp_push_server", rtmp_server_address);
	rtmp_push_server.SetThreadNum(2);

	HttpPullServer http_pull_server(&loop, "http_pull_server", client_server_address);
	http_pull_server.SetThreadNum(4);

	http_pull_server.SetGetPushConnCallback([server = &rtmp_push_server](auto&& PH1)
	{
		return server->GetPushConnByUrl(PH1);
	});

	rtmp_push_server.Start();
	http_pull_server.Start();

	loop.Loop();
}
