#ifndef SYB_WEBSOCKET_HPP
#define SYB_WEBSOCKET_HPP

#include <winsock2.h>
#include <thread>
#include <string>
#include <queue>
#include <mutex>

#include "ITcpSocket.hpp"
#include "easywsclient/easywsclient.hpp"
#include "core/SybDebug.hpp"


namespace syb
{
	class WebSocket : public ITcpSocket
	{
	public:
		~WebSocket();

		bool Connect(const char* address, const char* port, const char* connection_flag);
		bool Send(const char* msg);
		std::queue<std::string> GetMessageQueue();

	private:
		void SendThread();
		void ReceiveThread();

		std::queue<std::string> m_RecvQueue;
		std::queue<std::string> m_SendQueue;

		std::thread m_threadReceive;
		std::thread m_threadSend;
		std::mutex m_mutexRecvQueue;
		std::mutex m_mutexSendQueue;
		std::timed_mutex m_mutexSock;

		easywsclient::WebSocket::pointer m_pSocket;
	};
} // namespace syb

#endif // SYB_WEBSOCKET_HPP