#ifndef SYB_IOCHANNEL_HPP
#define SYB_IOCHANNEL_HPP

#include <string>
#include <queue>

#include "core/SybDef.hpp"
#include "ITcpSocket.hpp"
#include "WebSocket.hpp"

namespace syb
{
	// ----------------------------------------------------------------------------
	// IOChannel
	// ----------------------------------------------------------------------------
	class IOChannel
	{
		friend class WebSocket;
	public:
		// ----------------------------------------------------------------------------
		// Connection targets
		// ----------------------------------------------------------------------------
		enum ConnectionTarget
		{
			SYB_LAB,
			SYB_VIS
		};
		static const ConnectionTarget DEF_CONN_TARGET = SYB_VIS;

		// ----------------------------------------------------------------------------
		// Tcp connection parameters
		// ----------------------------------------------------------------------------
		struct TcpConnectionParams
		{
			char *address;
			char *port;
		};

		// ----------------------------------------------------------------------------
		// Connection states
		//
		// STATE_DISCONNECTED is logically equivalent to false(not connected)
		// ----------------------------------------------------------------------------
		enum State
		{
			STATE_DISCONNECTED,
			STATE_CONNECTED
		};

		// ----------------------------------------------------------------------------
		// Generic to platform specific switches - currently defaults to the WebSockets wrapper
		// ----------------------------------------------------------------------------
		//#ifdef WIN32
		//#define TcpSocket WinTcpSocket
		//#endif
		#define TcpSocket WebSocket

	public:
		IOChannel(const char* connection_flag, ConnectionTarget target = DEF_CONN_TARGET, TcpConnectionParams *cParam = nullptr);
		~IOChannel();
		
		bool Send(const char *msg);
		std::queue<std::string> GetMessageQueue();
		const bool &GetState() const;

	private:
		State m_State;
		ConnectionTarget m_Target;
		ITcpSocket *m_pSocket;
	};
} // namespace syb

#endif // SYB_IOCHANNEL_HPP