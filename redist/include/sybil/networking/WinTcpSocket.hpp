#ifndef SYB_WIN_TCPSOCKET_HPP
#define SYB_WIN_TCPSOCKET_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "IODevice.hpp"


namespace syb
{
	class WinTcpSocket : public IODevice
	{
	public:
		WinTcpSocket();
		~WinTcpSocket();

		void Connect(const std::string& target, const std::string& connection_flag, const std::string& token);
		void Send(const std::string& msg);
		std::queue<std::string> GetMsgQueue();

	private:
		void SendThread();
		void ReceiveThread();
		void HandleBuffer(char buffer[BUFSIZE], int cbReceived);

		SOCKET m_Socket;
		struct MsgStump
		{
			std::string msg;
			std::size_t msgLength;
		} m_Stump;
	};
} // namespace syb

#endif // SYB_WIN_TCPSOCKET_HPP