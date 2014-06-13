#ifndef SYB_WIN_TCPSOCKET_HPP
#define SYB_WIN_TCPSOCKET_HPP

#ifndef SYB_DEF_HPP
#include "core/SybDef.hpp"
#endif
#include "ITcpSocket.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


namespace syb
{
	/// A Win32 WinSock wrapper class
	class WinTcpSocket
	{
	public:
		WinTcpSocket(const char *server_req, const char *address = SOCK_DEF_ADDRESS, const char *port = SOCK_DEF_PORT);
		~WinTcpSocket();
		void Connect();
		void Kill();

	private:
		void SendThread();
		void ReceiveThread();
		SOCKET m_Socket;
	};
} // namespace syb

#endif // SYB_WIN_TCPSOCKET_HPP