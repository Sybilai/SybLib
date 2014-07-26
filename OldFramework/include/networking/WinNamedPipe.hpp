#ifndef SYB_WIN_NAMEDPIPE_HPP
#define SYB_WIN_NAMEDPIPE_HPP

#include <Windows.h>

#include "IODevice.hpp"


namespace syb
{
	class WinNamedPipe : public IODevice
	{
	public:
		WinNamedPipe();
		~WinNamedPipe();

		void Connect(const std::string &target, const std::string &connection_flag);
		void Send(const std::string msg);
		std::queue<std::string> GetMsgQueue();

	private:
		void SendThread();
		void ReceiveThread();
		HANDLE m_hPipe;
	};
} // namespace syb

#endif // SYB_WIN_NAMEDPIPE_HPP