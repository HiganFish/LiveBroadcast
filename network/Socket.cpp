#include <unistd.h>

#include "network/SocketOps.h"
#include "network/Socket.h"

Socket::Socket(int sockfd) :
	sockfd_(sockfd)
{

}

Socket::~Socket()
{
	if (sockfd_ > 0)
	{
		socketops::Close(sockfd_);
		sockfd_ = -1;
	}
}

int Socket::GetSockFd() const
{
	return sockfd_;
}

void Socket::SetSockfd(int sockfd)
{
	sockfd_ = sockfd;
}

void Socket::SetReusePort()
{
#ifdef _WIN32
#else
	socketops::SetReusePort(sockfd_);
#endif
}

void Socket::SetReuseAddr()
{
	socketops::SetReuseAddr(sockfd_);
}

void Socket::Bind(const InetAddress& address)
{
	socketops::Bind(sockfd_, *address.GetSockAddr());
}

void Socket::Listen()
{
	socketops::Listen(sockfd_);
}

int Socket::Accept(InetAddress* address)
{

	return socketops::Accept(sockfd_, address->GetSockAddr());
}

ssize_t Socket::Send(const char* data, size_t length)
{
	return socketops::Send(sockfd_, data, length);
}

void Socket::ShutdownWrite()
{
	socketops::ShutdownWrite(sockfd_);
}

