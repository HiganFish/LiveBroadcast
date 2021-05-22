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

std::string HttpPullConnection::GetConnectionName() const
{
	return connection_ptr_->GetConnectionName();
}

constexpr char response_header[] = "HTTP/1.1 200 OK\r\n"
                              "Server: FISH_LIVE\r\n"
                              "Date: Sun, 29 Nov 2020 15:30:42 GMT\r\n"
                              "Content-Type: video/x-flv\r\n"
                              "Transfer-Encoding: chunked\r\n"
                              "Connection: keep-alive\r\n"
                              "Access-Control-Allow-Origin: *\r\n\r\n";

void HttpPullConnection::SendHeaderOnConnection(const Buffer& buffer)
{
	std::string length_rn = Format::ToHexStringWithCrlf(buffer.ReadableLength());
	Buffer temp_buffer;

	temp_buffer.AppendData(response_header);

	temp_buffer.AppendData(length_rn);
	temp_buffer.AppendData(buffer);
	temp_buffer.AppendData("\r\n");

	connection_ptr_->Send(temp_buffer);
}

void HttpPullConnection::AddFlvTag(const FlvTagPtr& flv_tag_ptr)
{
	std::string length_rn = Format::ToHexStringWithCrlf(flv_tag_ptr->GetSumSize());
	Buffer temp_buffer;
	temp_buffer.AppendData(length_rn);
	temp_buffer.AppendData(flv_tag_ptr->GetHeader(), FlvTag::FLV_TAG_HEADER_LENGTH);
	temp_buffer.AppendData(flv_tag_ptr->GetBody());
	temp_buffer.AppendData("\r\n");

	connection_ptr_->Send(temp_buffer);
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