#ifndef SYB_IODEVICE_HPP
#define SYB_IODEVICE_HPP

#include <queue>
#include <mutex>
#include <string>
#include <thread>

//#include "../core/SybDebug.hpp"


namespace syb
{
	class IODevice
	{
	public:
		virtual ~IODevice() { }
		virtual void Connect(const std::string &target, const std::string &connection_flag) = 0;
		virtual void Send(const std::string msg) = 0;
		virtual std::queue<std::string> GetMsgQueue() = 0;

	protected:
		static const int BUFSIZE = 1024;

		IODevice() { }
		virtual void SendThread() = 0;
		virtual void ReceiveThread() = 0;

		std::queue<std::string> m_RecvQueue;
		std::queue<std::string> m_SendQueue;

		std::thread m_threadReceive;
		std::thread m_threadSend;
		std::mutex m_mutexRecvQueue;
		std::mutex m_mutexSendQueue;
		//std::timed_mutex m_mutexSock;
	};
} // namespace syb

#endif // SYB_IODEVICE_HPP