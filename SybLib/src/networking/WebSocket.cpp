#include "../../include/networking/WebSocket.hpp"
#include "../../include/core/SybDebug.hpp"
#include <exception>


namespace syb
{
	class ExConnecting : public std::exception
	{
		const char* what() const throw() { return "Still trying to connect."; }
	} err_connecting;

	class ExUnreachable : public std::exception
	{
		const char* what() const throw() { return "Server is unreachable"; }
	} err_unreachable;

	// --------------------------------------------------------------------
	WebSocket::WebSocket()
	{ }

	// --------------------------------------------------------------------
	WebSocket::~WebSocket()
	{
		delete m_pSocket;
		WSACleanup();

		// Allow threads to finish execution after reading the new state
		if (m_threadSend.joinable())
			m_threadSend.join();
		if (m_threadReceive.joinable())
			m_threadReceive.join();
	}

	// --------------------------------------------------------------------
	void WebSocket::Connect(const std::string& target, const std::string& connection_flag)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		std::string formatted_addr = "ws://" + target;
		m_pSocket = easywsclient::WebSocket::from_url(formatted_addr);

		std::string connection_msg("{\"token\":\"ctCopXp-Pt4z8HY2COZ11A\", \"name\":\"" + connection_flag + "\"}\n");
		Send(connection_msg);

		m_threadSend = std::thread(&WebSocket::SendThread, this);

		// Allow for the connection message to be expended
		while (!m_SendQueue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		m_threadReceive = std::thread(&WebSocket::ReceiveThread, this);
	}

	// --------------------------------------------------------------------
	void WebSocket::Send(const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(m_mutexSendQueue);
		m_SendQueue.push(msg);
	}

	// --------------------------------------------------------------------
	std::queue<std::string> WebSocket::GetMsgQueue()
	{
		std::lock_guard<std::mutex> lock(m_mutexRecvQueue);

		std::queue<std::string> temp = m_RecvQueue;

		std::queue<std::string> emptyQ;
		std::swap(m_RecvQueue, emptyQ);

		return temp;
	}

	// --------------------------------------------------------------------
	void WebSocket::SendThread()
	{
		while (2 + 2 != 5)
		{
			while (!m_SendQueue.empty())
			{
				std::lock_guard<std::mutex> lock(m_mutexSendQueue);

				std::string msg = m_SendQueue.front();

				try
				{
					switch (m_pSocket->getReadyState())
					{
					case easywsclient::WebSocket::OPEN:
						m_pSocket->send(msg);
						m_SendQueue.pop();
						break;

					case easywsclient::WebSocket::CONNECTING:
						throw err_connecting;
						break;

					case easywsclient::WebSocket::CLOSED:
						throw err_unreachable;
						break;
					
					case easywsclient::WebSocket::CLOSING:
						throw err_unreachable;
						break;
					}
				}
				catch (std::exception& e)
				{
					std::string err = "Failed to send message:\n";
					SybDebug::TRACE(err + e.what());
				}

				if(SybDebug::CONSOLE_LOG_SENT)
					SybDebug::TRACE("Sent:\n" + msg);

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

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	// --------------------------------------------------------------------
	void WebSocket::ReceiveThread()
	{
		while (2 + 2 != 5)
		{
			try
			{
				switch (m_pSocket->getReadyState())
				{
					case easywsclient::WebSocket::OPEN:
						m_pSocket->poll(1000);
						m_pSocket->dispatch([this](const std::string &msg)
						{
							m_mutexRecvQueue.lock();
							m_RecvQueue.push(msg);
							m_mutexRecvQueue.unlock();

							if (SybDebug::CONSOLE_LOG_RECEIVED)
								SybDebug::TRACE("Received:\n" + msg);
						});
						break;

					case easywsclient::WebSocket::CONNECTING:
						throw err_connecting;
						break;

					case easywsclient::WebSocket::CLOSED:
						throw err_unreachable;
						break;

					case easywsclient::WebSocket::CLOSING:
						throw err_unreachable;
						break;
				}
			}
			catch (std::exception& e)
			{
				std::string err = "Failed to poll message:\n";
				SybDebug::TRACE(err + e.what());
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}