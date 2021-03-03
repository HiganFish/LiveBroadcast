#ifndef NETWORK_SOCKETOPS
#define NETWORK_SOCKETOPS

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

namespace socketops
{
	int CreateDefaultSocket(int family);

	int CreateNonblockSocket(int family);

	int Htons(int port);

	void Bind(int sockfd, const sockaddr& address);

	int Connect(int sockfd, const sockaddr& address);

	void Listen(int sockfd);

	void SetReusePort(int sockfd);

	void SetReuseAddr(int sockfd);

	int Accept(int sockfd, struct sockaddr* address);

	ssize_t Send(int sockfd, const char* data, size_t length);

	void Close(int sockfd);

	void ShutdownWrite(int sockfd);

	sockaddr_in6 GetPeerAddr(int sockfd);

	int InetPton(int af, const char* from, void* to);

	/**
	 * 将地址转换为网络地址
	 * @param name 域名或者ip地址
	 * @param addr 网络地址 ipv4
	 * @return 转换成功true
	 */
	bool NameToAddr4(const std::string& name, in_addr* addr);

	/**
	* 将地址转换为网络地址
	* @param name 域名或者ip地址
	* @param addr 网络地址 ipv6
	* @return 转换成功true
	*/
	void NameToAddr6(const std::string& name, in6_addr* addr);
}

#endif // !NETWORK_SOCKETOPS