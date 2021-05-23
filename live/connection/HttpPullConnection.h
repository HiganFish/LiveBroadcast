//
// Created by rjd67 on 2020/11/30.
//

#ifndef LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H
#define LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H

#include "network/TcpConnection.h"
#include "utils/codec/FlvCodec.h"
#include "live/connection/PullConnection.h"

class HttpPullConnection : public PullConnection
{
public:
	explicit HttpPullConnection(const TcpConnectionPtr& connection_ptr);

	std::string GetConnectionName() const override;

	/**
	 * 在连接刚建立时 需要发送一次包含元数据的头部
	 * @param buffer
	 */
	void SendHeaderOnConnection(const Buffer& buffer) override;

	/**
	 * 向客户端添加新的 flv_tag_ptr 并进行发送
	 * @param  tag_ptr 新的flv_tag指针
	 */
	void AddFlvTag(const FlvTagPtr& flv_tag_ptr) override;

private:

	TcpConnectionPtr connection_ptr_;

	void OnConnection(const TcpConnectionPtr& connection_ptr);
};


#endif //LIVEBROADCASTSERVER_HTTPPULLCONNECTION_H
