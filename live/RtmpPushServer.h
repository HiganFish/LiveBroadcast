//
// Created by rjd67 on 2021/5/23.
//

#ifndef LIVEBROADCASTSERVER_RTMPPUSHSERVER_H
#define LIVEBROADCASTSERVER_RTMPPUSHSERVER_H

#include "network/TcpServer.h"
#include "live/connection/RtmpPushConnection.h"

typedef std::function<RtmpPushConnectionPtr(const std::string&)> GetPushConnCallback;
class RtmpPushServer
{
public:

	RtmpPushServer(EventLoop* loop, const std::string& server_name, const InetAddress& address);

	void SetThreadNum(int thread_num);

	void Start();

	RtmpPushConnectionPtr GetPushConnByUrl(const std::string& url);

private:

	EventLoop* loop_;

	InetAddress server_address_;

	TcpServer server_;

	AuthenticationCallback authentication_callback_;

	std::map<std::string, RtmpPushConnectionPtr> rtmp_connection_map_;

	void OnShakeHandSuccess(const RtmpPushConnectionPtr& server_connection);

	void RemoveRtmpPushConnection(const TcpConnectionPtr& connection_ptr);

	/** 主播建立连接后的回调函数*/
	void OnConnection(const TcpConnectionPtr& connection_ptr);
};

#endif //LIVEBROADCASTSERVER_RTMPPUSHSERVER_H
