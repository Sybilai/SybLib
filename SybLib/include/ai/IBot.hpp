#ifndef SYB_BOT_HPP
#define SYB_BOT_HPP

#include <string>

#include "networking/IOChannel.hpp"
#include "games/bomberman/BombWorld.hpp"


namespace syb
{
	class IBot
	{
	public:
		IBot(std::string name) :
			m_Name(name),
			m_pPos(nullptr),
			m_pMe(nullptr),
			m_pWorld(nullptr)
		{
		}

		const std::string &GetName() const { return m_Name; }
		void SetId(const int &id) { m_MyId = id; }
		const int &GetId() { return m_MyId; }
		void AttachIOChannel(IOChannel *pIo) { m_pIO = pIo; }
		void AttachWorld(BombWorld *pWorld) { m_pWorld = pWorld; }

		virtual void Update() = 0;

		Bomberman *m_pMe;
		Vec2 *m_pPos;

	protected:
		void Send(std::string event_type, std::string *data = nullptr)
		{
			if (event_type == "bomb")
			{
				std::string msg = "{\"event\":\"bomb\"}\n";
				m_pIO->Send(msg.c_str());
			}
			else if (event_type == "move")
			{
				std::string msg = "{\"event\":\"move\", \"direction\": [" + *data + "]}\n";
				m_pIO->Send(msg.c_str());
			}
		}

		std::string m_Name;
		int m_MyId;
		BombWorld *m_pWorld;

	private:
		IOChannel *m_pIO;
	};
} // namespace syb

#endif // SYB_BOT_HPP