//
// Created by rjd67 on 2020/11/30.
//

#include "network/connection/HttpPullConnection.h"
#include "utils/Format.h"

HttpPullConnection::HttpPullConnection(const TcpConnectionPtr& connection_ptr):
	connection_ptr_(connection_ptr)
{
	connection_ptr_->SetConnectionCallback(
			[this](auto&& PH1){OnConnection(PH1);});
}

HttpPullConnection::~HttpPullConnection()
{

}

std::string HttpPullConnection::GetConnectionName() const
{
	return connection_ptr_->GetConnectionName();
}

void HttpPullConnection::SendHeader(const Buffer* buffer)
{
	connection_ptr_->Send(buffer);
}

void HttpPullConnection::AddNewTag(const FlvTagPtr& flv_tag_ptr)
{
	std::string length_rn = Format::ToHexStringWithCrlf(flv_tag_ptr->GetSumSize());

	connection_ptr_->Send(length_rn);
	connection_ptr_->Send(flv_tag_ptr->GetHeader(), FlvTag::FLV_TAG_HEADER_LENGTH);
	connection_ptr_->Send(flv_tag_ptr->GetBody());
	connection_ptr_->Send("\r\n");
}

void HttpPullConnection::SetCloseConnectionCallback(const ConnectionCallback& callback)
{
	close_connection_callback_ = callback;
}

void HttpPullConnection::OnConnection(const TcpConnectionPtr& connection_ptr)
{
	if (!connection_ptr->Connected())
	{
		assert(connection_ptr->GetConnectionName() == connection_ptr_->GetConnectionName());
		if (close_connection_callback_)
		{
			close_connection_callback_(connection_ptr);
		}
	}
}