//
// Created by rjd67 on 2021/5/23.
//

#ifndef LIVEBROADCASTSERVER_HTTPPULLSERVER_H
#define LIVEBROADCASTSERVER_HTTPPULLSERVER_H

#include "network/TcpServer.h"
#include "live/connection/HttpPullConnection.h"
#include "live/RtmpPushServer.h"

class HttpPullServer
{
public:

	HttpPullServer(EventLoop* loop, const std::string& server_name, const InetAddress& address);

	void SetThreadNum(int thread_num);

	void Start();

	void SetGetPushConnCallback(const GetPushConnCallback& callback);

private:

	EventLoop* loop_;
	InetAddress server_address_;
	TcpServer server_;

	GetPushConnCallback  get_push_conn_callback_;

	void OnClientMessage(const TcpConnectionPtr& connection_ptr, Buffer* buffer, Timestamp timestamp);
};

#endif //LIVEBROADCASTSERVER_HTTPPULLSERVER_H
