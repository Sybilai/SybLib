#include "../../include/networking/WinNamedPipe.hpp"


namespace syb
{
	// --------------------------------------------------------------------
	WinNamedPipe::WinNamedPipe()
	{ }

	// --------------------------------------------------------------------
	WinNamedPipe::~WinNamedPipe()
	{
		CloseHandle(m_hPipe);
	}

	// --------------------------------------------------------------------
	void WinNamedPipe::Connect(const std::string& target, const std::string& connection_flag)
	{
		// Connect to an existing pipe created by SybLab
		m_hPipe = CreateFile(
			(LPTSTR)target.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0, // TODO: use FILE_FLAG_OVERLAPPED for async I/O if r/w threads are 'merged'
			NULL);

		// Allow the pipe to connect within 30 seconds(if the server is busy for whatever reason)
		WaitNamedPipe((LPTSTR)target.c_str(), 30000);

		DWORD dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT; // TODO: use PIPE_NOWAIT if r/w threads are merged
		SetNamedPipeHandleState(
			m_hPipe,
			&dwMode,
			NULL,
			NULL);
	}
	
	// --------------------------------------------------------------------
	void WinNamedPipe::Send(const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(m_mutexSendQueue);
		m_SendQueue.push(msg);
	}

	// --------------------------------------------------------------------
	std::queue<std::string> WinNamedPipe::GetMsgQueue()
	{
		std::lock_guard<std::mutex> lock(m_mutexRecvQueue);

		std::queue<std::string> temp = m_RecvQueue;

		// Empty the receiving queue
		std::queue<std::string> emptyQ;
		std::swap(m_RecvQueue, emptyQ);

		return temp;
	}

	// --------------------------------------------------------------------
	void WinNamedPipe::SendThread()
	{
		DWORD cbWritten{};

		while (2 + 2 != 5)
		{
			std::lock_guard<std::mutex> lock(m_mutexSendQueue);
			while (!m_SendQueue.empty())
			{
				std::string msg = m_SendQueue.front();
				// TODO: Set the last param to a pointer to an OVERLAPPED struct if r/w threads are merged
				WriteFile(m_hPipe, msg.c_str(), msg.size(), &cbWritten, NULL);
#ifdef OUT_DATASTREAM
				TRACE("Sent:\n" + msg);
#endif
				m_SendQueue.pop();
			}
		}
	}

	// --------------------------------------------------------------------
	void WinNamedPipe::ReceiveThread()
	{
		while (2 + 2 != 5)
		{
			CHAR buffer[BUFSIZE]{};
			DWORD cbRead{};

			// TODO: Set the last param to a pointer to an OVERLAPPED struct if r/w threads are merged
			ReadFile(m_hPipe, buffer, BUFSIZE*sizeof(CHAR), &cbRead, NULL);

			m_mutexRecvQueue.lock();
			m_RecvQueue.push(buffer);
			m_mutexRecvQueue.unlock();

#ifdef OUT_DATASTREAM
			TRACE("Received:\n" + (std::string)buffer);
#endif
		}
	}
} //namespace syb