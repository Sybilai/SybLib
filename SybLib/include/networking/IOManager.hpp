#ifndef SYB_IOMANAGER_HPP
#define SYB_IOMANAGER_HPP

#include <string>
#include <memory>
#include <queue>

#ifdef WIN32
#include "WinTcpSocket.hpp"
#include "WinNamedPipe.hpp"
#else
#include "BsdTcpSocket.hpp"
#endif
#include "WebSocket.hpp"

//#include "../events/IEventListener.hpp"


namespace syb
{
	/*namespace ecospace
	{
		class IOEventListener
		{
		public:
			IOEventListener()
			void ReceiveEvent(IEvent*);
		};
	}*/

	/// Handles data transfer between the current AI instance and an external authority:
	/// either SybLab(the local testbench) or SybVis(the multiplayer server)
	class IOManager //: public ecospace::IOEventListener
	{
	public:
		enum ConnectionTarget
		{
			SYB_LAB,
			SYB_VIS
		};
		static const ConnectionTarget DEFAULT_TARGET = SYB_VIS;
		static const std::string DEFAULT_LAB_PARAMS;
		static const std::string DEFAULT_VIS_PARAMS;

		/// TCP specific connection parameters
		struct TcpConnectionParams
		{
			std::string address;
			std::string port;

			operator std::string() const;
		};

		/// Connection states
		/// STATE_DISCONNECTED and STATE_CONNECTING are logically equivalent to false(not connected)
		enum State
		{
			STATE_DISCONNECTED = 0,
			STATE_CONNECTING = 0,
			STATE_CONNECTED
		};

		/// Generic to platform specific switches
//#ifdef WIN32
//		using TcpSocket = WinTcpSocket;
//#else
//		using TcpSocket = BsdTcpSocket;
//#endif
		// Currently the most reliable
		using TcpSocket = WebSocket;

	public:
		// Leave definitions empty. Loading/Unloading are handled by Init()/Shutdown()
		IOManager();
		~IOManager();

		void Init(const std::string &connection_flag, const ConnectionTarget &target = DEFAULT_TARGET, std::string *params = nullptr);
		void Shutdown();

		void SendMsg(const std::string &msg);
		std::queue<std::string> GetMsgQueue();
		//const bool &GetState() const;
		bool GetState();

	private:
		State m_State;
		std::unique_ptr<IODevice> m_pDevice;
	};
} // namespace syb

#endif // SYB_IOMANAGER_HPP