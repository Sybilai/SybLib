#include "../../include/networking/IOManager.hpp"

namespace syb
{
	const std::string IOManager::DEFAULT_LAB_PARAMS = "\\\\.\\pipe\\syblabpipe";
	const std::string IOManager::DEFAULT_VIS_PARAMS = "sybilai.com:8124";

	// --------------------------------------------------------------------
	IOManager::TcpConnectionParams::operator std::string() const
	{
		return address + ':' + port;
	}

	// --------------------------------------------------------------------
	IOManager::IOManager() :
		m_State(STATE_DISCONNECTED),
		m_pDevice(nullptr)
	{ }

	// --------------------------------------------------------------------
	IOManager::~IOManager()
	{ 
		Shutdown();
	}

	// --------------------------------------------------------------------
	void IOManager::Init(const std::string &connection_flag, const ConnectionTarget &target, std::string *params, const std::string& token)
	{
		switch (target)
		{
		case SYB_LAB:
			m_pDevice = std::make_unique<WinNamedPipe>();
			m_State = STATE_CONNECTING;
			if (params)
				m_pDevice->Connect(*params, connection_flag, token);
			else
				m_pDevice->Connect(DEFAULT_LAB_PARAMS, connection_flag, token);
			break;
		case SYB_VIS:
			m_pDevice = std::make_unique<TcpSocket>();
			m_State = STATE_CONNECTING;
			if (params)
				m_pDevice->Connect(*params, connection_flag, token);
			else
				m_pDevice->Connect(DEFAULT_VIS_PARAMS, connection_flag, token);

			m_State = STATE_CONNECTED;
			break;
		}
	}

	// --------------------------------------------------------------------
	void IOManager::Shutdown()
	{
		// this pointer is way too smart for this type of stuff
		// delete m_pDevice;
	}

	// --------------------------------------------------------------------
	void IOManager::SendMsg(const std::string &msg)
	{
		if (m_State == STATE_CONNECTED)
			m_pDevice->Send(msg);
	}

	// --------------------------------------------------------------------
	std::queue<std::string> IOManager::GetMsgQueue()
	{
		return m_pDevice->GetMsgQueue();
	}

	// --------------------------------------------------------------------
	//const bool &IOManager::GetState() const
	bool IOManager::GetState()
	{
#ifdef _MSC_VER
// TODO: place this before including IOManager
//#pragma warning( disable : C48000 ) // forcing value to bool
#endif
		bool state = false;
		if (m_State == State::STATE_CONNECTED)
			state = true;

		return state;
		//return m_State;
	}
}