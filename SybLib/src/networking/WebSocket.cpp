#include "networking/WebSocket.hpp"


namespace syb
{
	WebSocket::~WebSocket()
	{
		delete m_pSocket;
		WSACleanup();
	}

	bool WebSocket::Connect(const char* address, const char* port, const char* connection_flag)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		std::string formattedAddr = "ws://" + (std::string)address + ":" + (std::string)port;
		m_pSocket = easywsclient::WebSocket::from_url(formattedAddr);

		std::string formConnMsg("{\"token\":\"ctCopXp-Pt4z8HY2COZ11A\", \"name\":\"" + (std::string)connection_flag + "\"}");
		Send(formConnMsg.c_str());

		m_threadSend = std::thread(&WebSocket::SendThread, this);

		// Allow for the connection message to be expended
		while (!m_SendQueue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		m_threadReceive = std::thread(&WebSocket::ReceiveThread, this);

		return true;
	}

	bool WebSocket::Send(const char* msg)
	{
		std::lock_guard<std::mutex> lock(m_mutexSendQueue);
		m_SendQueue.push(msg);
		return true;
	}

	std::queue<std::string> WebSocket::GetMessageQueue()
	{
		std::lock_guard<std::mutex> lock(m_mutexRecvQueue);

		std::queue<std::string> temp = m_RecvQueue;

		std::queue<std::string> emptyQ;
		std::swap(m_RecvQueue, emptyQ);

		return temp;
	}

	void WebSocket::SendThread()
	{
		while (2 + 2 != 5)
		{
			while (!m_SendQueue.empty())
			{
				std::lock_guard<std::mutex> lock(m_mutexSendQueue);

				std::string msg = m_SendQueue.front();
				m_pSocket->send(msg);
#ifdef OUT_DATASTREAM
				TRACE("Sent:\n" + msg);
#endif
				m_SendQueue.pop();

				/*if (m_mutexSock.try_lock())
				{
					std::string msg = m_SendQueue.front();
					m_pSocket->send(msg);
					m_mutexSock.unlock();
#ifdef OUT_DATASTREAM
					TRACE("Sent:\n" + msg);
#endif
					m_SendQueue.pop();
				}*/
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	void WebSocket::ReceiveThread()
	{
		while (m_pSocket->getReadyState() != easywsclient::WebSocket::CLOSED) 
		{
			// fu.ck.th.is. 
			m_pSocket->poll(1000);
			m_pSocket->dispatch([this](const std::string &msg)
			{
				m_mutexRecvQueue.lock();
				m_RecvQueue.push(msg);
				m_mutexRecvQueue.unlock();
				TRACE("Received:\n" + msg);
			});
		}
	}
}
