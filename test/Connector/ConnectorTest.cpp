//
// Created by rjd67 on 2021/1/14.
//

#include <network/TcpClient.h>
#include <network/TcpConnection.h>
#include <utils/Logger.h>

void OnNewConnection(const TcpConnectionPtr& connection)
{
	if (connection->Connected())
	{

		connection->Send("11111\n");
	}
	else
	{
		LOG_INFO << "connection: " << connection->GetConnectionName() << " close";
	}
}

void OnNewMessage(const TcpConnectionPtr& connection, Buffer* buffer, Timestamp stamp)
{
	LOG_INFO << "connection: " << connection->GetConnectionName()
			 << " send\n" << buffer->ReadAllAsString();
}

int main()
{
	EventLoop loop;
	InetAddress address("df.lsmg.xyz", 4000, false);
	TcpClient tcp_client(&loop, address, "TestConnector");
	tcp_client.SetConnectCallback(OnNewConnection);
	tcp_client.SetNewMessageCallback(OnNewMessage);
	tcp_client.SetReConnect(true);

	tcp_client.Connect();
	loop.Loop();
}