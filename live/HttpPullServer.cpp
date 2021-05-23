//
// Created by rjd67 on 2021/5/23.
//
#include "utils/Format.h"
#include "utils/Logger.h"
#include "live/HttpPullServer.h"
#include "live/connection/HttpPullConnection.h"

HttpPullServer::HttpPullServer(EventLoop* loop, const std::string& server_name, const InetAddress& address):
	loop_(loop),
	server_address_(address),
	server_(loop, server_name, address)
{
	server_.SetNewMessageCallback([this](auto&& PH1, auto&& PH2, auto&& PH3)
	{
		OnClientMessage(PH1, PH2, PH3);
	});
}

void HttpPullServer::SetThreadNum(int thread_num)
{
	server_.SetThreadNum(thread_num);
}

void HttpPullServer::Start()
{
	server_.Start();
}

void HttpPullServer::SetGetPushConnCallback(const GetPushConnCallback& callback)
{
	get_push_conn_callback_ = callback;
}

void HttpPullServer::OnClientMessage(const TcpConnectionPtr& connection_ptr, Buffer* buffer, Timestamp timestamp)
{
	std::string connection_data = buffer->ReadAllAsString();

	/**
	 * 获取HTTP请求中的url 根据上面设置的映射关系 同样获取url
	 * 来获取到对应的server_connection 加入其中
	 */
	std::string url = Format::GetUrl(connection_data);

	RtmpPushConnectionPtr server_connection;
	if (get_push_conn_callback_)
	{
		server_connection = get_push_conn_callback_(url);
	}

	if (server_connection)
	{
		LOG_INFO << "connection: " << connection_ptr->GetConnectionName()
		         << ", request url: " << url << " success";

		server_connection->AddClientConnection(
			std::make_shared<HttpPullConnection>(connection_ptr));
	}
	else
	{
		LOG_INFO << "connection: "<< connection_ptr->GetConnectionName()
		         << ", request url: " << url << " failed";

		connection_ptr->Shutdown();
	}
}
