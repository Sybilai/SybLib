#include "networking/WinTcpSocket.hpp"

#include <iostream>

namespace syb
{
	WinTcpSocket::WinTcpSocket(const char *server_req, const char *address, const char *port)
	{
		Connect();

		Send(server_req);
		m_threadSend = std::thread(&WinTcpSocket::SendThread, this);

		// Allow for the connection message to be expended
		while (!m_SendQueue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		m_mutexState.lock();
		m_State = SOCK_STATE_CONNECTED;
		m_mutexState.unlock();

		m_threadReceive = std::thread(&WinTcpSocket::ReceiveThread, this);
	}
	
	WinTcpSocket::~WinTcpSocket()
	{
		m_mutexState.lock();
		if (m_State == SOCK_STATE_CONNECTED)
			Kill();
		m_mutexState.unlock();
		
		// Allow threads to finish execution after reading the new state
		if (m_threadSend.joinable())
			m_threadSend.join();
		if (m_threadReceive.joinable())
			m_threadReceive.join();
	}

	void WinTcpSocket::Connect()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		struct addrinfo *result = nullptr, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		
		getaddrinfo(m_Address, m_Port, &hints, &result);

		m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		connect(m_Socket, result->ai_addr, (int)result->ai_addrlen);

		freeaddrinfo(result);
		// Set socking I/O mode to blocking
		// It is blocking by default, but due to future adventures into the realm of WinSock, this may change 
		ioctlsocket(m_Socket, FIONBIO, 0);
	}

	void WinTcpSocket::Kill()
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		WSACleanup();

		std::lock_guard<std::mutex> lock(m_mutexState);
		m_State = SOCK_STATE_DISCONNECTED;
	}

	void WinTcpSocket::SendThread()
	{
		m_mutexState.lock();
		bool bConnected = m_State;
		m_mutexState.unlock();

		while (bConnected)
		{
			while (!m_SendQueue.empty())
			{
				std::lock_guard<std::mutex> lock(m_mutexSendQueue);

				std::string msg = m_SendQueue.front();
				send(m_Socket, msg.c_str(), msg.length(), 0);
				std::cout << "Sent:\n" << msg << "\n-------------------------------\n";
				m_SendQueue.pop();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
			std::lock_guard<std::mutex> lock(m_mutexState);
			bConnected = m_State;
		}
	}

	void WinTcpSocket::ReceiveThread()
	{
		m_mutexState.lock();
		bool bConnected = m_State;
		m_mutexState.unlock();

		while (bConnected)
		{
			char recvBuf[SOCK_BUF_LEN];
			int recvBytes = recv(m_Socket, recvBuf, SOCK_BUF_LEN, 0);
			if (recvBytes > 0)
				HandleRecvBuffer(recvBuf, recvBytes);

			std::lock_guard<std::mutex> lock(m_mutexState);
			bConnected = m_State;
		}
	}
} // namespace syb