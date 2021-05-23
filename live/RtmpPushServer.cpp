//
// Created by rjd67 on 2021/5/23.
//

#include "live/RtmpPushServer.h"
#include "utils/Logger.h"

bool DefaultAuthenticateCallback(const std::string& username, const std::string& password)
{
	return true;
}

RtmpPushServer::RtmpPushServer(EventLoop* loop, const std::string& server_name, const InetAddress& address):
	loop_(loop),
	server_address_(address),
	server_(loop, server_name, address),
	authentication_callback_(DefaultAuthenticateCallback)
{
	server_.SetConnectionCallback([this](auto&& PH1)
	{
		OnConnection(PH1);
	});
}

void RtmpPushServer::SetThreadNum(int thread_num)
{
	server_.SetThreadNum(thread_num);
}

void RtmpPushServer::Start()
{
	server_.Start();
}

RtmpPushConnectionPtr RtmpPushServer::GetPushConnByUrl(const std::string& url)
{
	auto iter = rtmp_connection_map_.find(url);

	if (iter == rtmp_connection_map_.end())
	{
		return {};
	}
	else
	{
		return iter->second;
	}
}

void RtmpPushServer::OnShakeHandSuccess(const RtmpPushConnectionPtr& server_connection)
{
	/**
	 * 将推流的url和server_connection关联起来 用于拉流的时候根据url获取对应的server_connection
	 *
	 * rtmp到server_connection 映射关系可以自己修改
	 */
	std::string path = server_connection->GetRtmpPath();
	rtmp_connection_map_[path] = server_connection;
	LOG_INFO << "server: " << server_connection->GetConnectionName()
	         << " bind to " << path;
}

void RtmpPushServer::RemoveRtmpPushConnection(const TcpConnectionPtr& connection_ptr)
{
	rtmp_connection_map_.erase(connection_ptr->GetConnectionName());
}

void RtmpPushServer::OnConnection(const TcpConnectionPtr& connection_ptr)
{
	if (connection_ptr->Connected())
	{
		auto server_connection = std::make_shared<RtmpPushConnection>(connection_ptr);

		// 连接建立后RtmpServerConnection内部会进行握手 然后握手成功后调用函数
		server_connection->SetShakeHandSuccessCallback([this](auto&& PH1)
		{
			OnShakeHandSuccess(PH1);
		});
		server_connection->SetAuthenticationCallback(authentication_callback_);

		/**
		 * 连接建立后 设置握手回调函数
		 */
		connection_ptr->SetNewMessageCallback(
			[server_connection](auto && PH1, auto && PH2, auto && PH3)
			{
				server_connection->OnConnectionShakeHand(PH1, PH2, PH3);
			});

		LOG_INFO << "connection: " << connection_ptr->GetConnectionName()
		         << " start shake hand";
	}
	else
	{
		RemoveRtmpPushConnection(connection_ptr);
	}
}