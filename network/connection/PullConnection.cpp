//
// Created by rjd67 on 2021/5/21.
//

#include "network/connection/PullConnection.h"

void PullConnection::SetCloseConnectionCallback(const ConnectionCallback& callback)
{
	close_connection_callback_ = callback;
}
