#include "networking/IOChannel.hpp"

namespace syb
{
	IOChannel::IOChannel(const char* connection_flag, ConnectionTarget target, TcpConnectionParams *cParam) :
		m_Target(target),
		m_State(STATE_DISCONNECTED)
	{	
		switch (m_Target)
		{
		case SYB_LAB:
			break;
		case SYB_VIS:
			if (cParam)
			{
				m_pSocket = new TcpSocket();
				if (m_pSocket->Connect(cParam->address, cParam->port, connection_flag))
					m_State = STATE_CONNECTED;
			}
			break;
		}
	}

	IOChannel::~IOChannel()
	{
		if (m_pSocket)
			delete m_pSocket;
	}

	bool IOChannel::Send(const char *msg)
	{
		if (m_State)
			switch (m_Target)
			{
			case SYB_LAB:
				break;
			case SYB_VIS:
				if (m_pSocket->Send(msg))
					return true;
				break;
			}

		return false;
	}

	std::queue<std::string> IOChannel::GetMessageQueue()
	{
		std::queue<std::string> q;
		switch (m_Target)
		{
		case SYB_LAB:
			break;
		case SYB_VIS:
			q = m_pSocket->GetMessageQueue();
			break;
		}
		return q;
	}

	const bool &IOChannel::GetState() const
	{
		return m_State;
	}
}