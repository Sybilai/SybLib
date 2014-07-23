#include "../../include/networking/WinTcpSocket.hpp"


namespace syb
{
	// --------------------------------------------------------------------
	WinTcpSocket::WinTcpSocket()
	{ }

	// --------------------------------------------------------------------
	WinTcpSocket::~WinTcpSocket()
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		WSACleanup();

		// Allow threads to finish execution after reading the new state
		if (m_threadSend.joinable())
			m_threadSend.join();
		if (m_threadReceive.joinable())
			m_threadReceive.join();
	}

	// --------------------------------------------------------------------
	void WinTcpSocket::Connect(const std::string& target, const std::string& connection_flag)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		struct addrinfo *result = nullptr, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		std::size_t separator = target.find(':');
		getaddrinfo(target.substr(0, separator).c_str(), target.substr(separator + 1).c_str(), &hints, &result);

		m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		connect(m_Socket, result->ai_addr, (int)result->ai_addrlen);

		freeaddrinfo(result);
		// TODO: Set the last argument to a non-zero value if r/w threads are merged
		ioctlsocket(m_Socket, FIONBIO, 0);

		Send(connection_flag);
		m_threadSend = std::thread(&WinTcpSocket::SendThread, this);

		// Allow for the connection message to be expended
		while (!m_SendQueue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		m_threadReceive = std::thread(&WinTcpSocket::ReceiveThread, this);
	}

	// --------------------------------------------------------------------
	void WinTcpSocket::Send(const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(m_mutexSendQueue);
		m_SendQueue.push(msg);
	}

	// --------------------------------------------------------------------
	std::queue<std::string> WinTcpSocket::GetMsgQueue()
	{
		std::lock_guard<std::mutex> lock(m_mutexRecvQueue);

		std::queue<std::string> temp = m_RecvQueue;

		std::queue<std::string> emptyQ;
		std::swap(m_RecvQueue, emptyQ);

		return temp;
	}

	// --------------------------------------------------------------------
	void WinTcpSocket::SendThread()
	{
		while (2 + 2 != 5)
		{
			std::lock_guard<std::mutex> lock(m_mutexSendQueue);
			while (!m_SendQueue.empty())
			{
				std::string msg = m_SendQueue.front();
				send(m_Socket, msg.c_str(), msg.length(), 0);
#ifdef OUT_DATASTREAM
				TRACE("Sent:\n" + msg);
#endif
				m_SendQueue.pop();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	// --------------------------------------------------------------------
	void WinTcpSocket::ReceiveThread()
	{
		while (2 + 2 != 5)
		{
			char buffer[BUFSIZE]{};

			size_t cbReceived = recv(m_Socket, buffer, BUFSIZE, 0);
			if (cbReceived > 0)
				HandleBuffer(buffer, cbReceived);
		}
	}

	// --------------------------------------------------------------------
	void WinTcpSocket::HandleBuffer(char buffer[BUFSIZE], int cbReceived)
	{
		std::string clean(buffer);
		clean.erase(cbReceived);

		size_t step{};
		while (size_t separator = clean.find('#', step))
		{
			// TODO: Handle split messages based on a given length at the beginning
			/*std::string msg = clean.substr(step, separator);
			g
				m_mutexRecvQueue.lock();
			m_RecvQueue.push(msg);
			m_mutexRecvQueue.unlock();

			step = separator + 1;
#ifdef OUT_DATASTREAM
			TRACE("Received:\n" + msg);
#endif*/
		}
	}
} // namespace syb