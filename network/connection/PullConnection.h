//
// Created by rjd67 on 2021/5/21.
//

#ifndef LIVEBROADCASTSERVER_PULLCONNECTION_H
#define LIVEBROADCASTSERVER_PULLCONNECTION_H

#include "network/Callback.h"
#include "utils/codec/FlvCodec.h"

class PullConnection
{
public:

	PullConnection() = default;
	virtual ~PullConnection() = default;

	virtual std::string GetConnectionName() const = 0 ;

	/**
	 * 在连接刚建立时 需要发送一次包含元数据的头部
	 * @param buffer
	 */
	virtual void SendHeaderOnConnection(const Buffer& buffer) = 0;

	/**
	 * 向客户端添加新的 flv_tag_ptr 并进行发送
	 * @param  tag_ptr 新的flv_tag指针
	 */
	virtual void AddFlvTag(const FlvTagPtr& flv_tag_ptr) = 0;

	void SetCloseConnectionCallback(const ConnectionCallback& callback);

protected:

	ConnectionCallback close_connection_callback_;
};

#endif //LIVEBROADCASTSERVER_PULLCONNECTION_H
