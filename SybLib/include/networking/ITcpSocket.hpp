#ifndef SYB_ITCPSOCKET_HPP
#define SYB_ITCPSOCKET_HPP

#include "core/SybDef.hpp"
#include <queue>
#include <string>


namespace syb
{
	class ITcpSocket
	{
	public:
		virtual ~ITcpSocket() { }
		virtual bool Connect(const char* address = SOCK_DEF_ADDRESS, const char* port = SOCK_DEF_PORT, const char* connection_flag = SOCK_DEF_CONNFLAG) = 0;
		virtual bool Send(const char* msg) = 0;
		virtual std::queue<std::string> GetMessageQueue() = 0;

	protected:
		virtual void SendThread() = 0;
		virtual void ReceiveThread() = 0;
	};
} // namespace syb

#endif // SYB_ITCPSOCKET_HPP