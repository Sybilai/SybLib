#ifndef SYB_WEBSOCKET_HPP
#define SYB_WEBSOCKET_HPP

#include <winsock2.h>
#include <string>
#include <queue>

#include "IODevice.hpp"
#include "easywsclient/easywsclient.hpp"


namespace syb
{
	class WebSocket : public IODevice
	{
	public:
		WebSocket();
		~WebSocket();

		void Connect(const std::string& target, const std::string& connection_flag);
		void Send(const std::string& msg);
		std::queue<std::string> GetMsgQueue();

	private:
		void SendThread();
		void ReceiveThread();

		easywsclient::WebSocket::pointer m_pSocket;
	};
} // namespace syb

#endif // SYB_WEBSOCKET_HPP