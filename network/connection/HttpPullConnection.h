//
// Created by rjd67 on 2020/11/30.
//

#ifndef LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H
#define LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H

#include "network/TcpConnection.h"
#include "utils/codec/FlvCodec.h"

class HttpPullConnection
{
public:
	explicit HttpPullConnection(const TcpConnectionPtr& connection_ptr);
	~HttpPullConnection();

	std::string GetConnectionName() const;


	/**
	 * 在连接刚建立时 需要发送一次包含元数据的头部
	 * @param buffer
	 */
	void SendHeader(const Buffer* buffer);

	/**
	 * 向客户端添加新的 flv_tag_ptr 并进行发送
	 * @param  tag_ptr 新的flv_tag指针
	 */
	void AddNewTag(const FlvTagPtr& flv_tag_ptr);

	void SetCloseConnectionCallback(const ConnectionCallback& callback);
private:

	TcpConnectionPtr connection_ptr_;
	ConnectionCallback close_connection_callback_;

	void OnConnection(const TcpConnectionPtr& connection_ptr);
};


#endif //LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H
