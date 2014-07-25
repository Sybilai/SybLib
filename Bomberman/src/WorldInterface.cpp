#include "../include/sybil/networking/IOManager.hpp"
#include "../include/WorldInterface.hpp"
#include <memory>


namespace boom
{
	World* WorldInterface::m_pWorld = nullptr;
	IBot* WorldInterface::m_pBot = nullptr;
	//unsigned int WorldInterface::m_BotId = -1;
	std::shared_ptr<Player> WorldInterface::m_pBotEntity = nullptr;

	// --------------------------------------------------------------------
	// Outbound requests
	// --------------------------------------------------------------------
	void WorldInterface::GoTo(const unsigned int& x, const unsigned int& y)
	{
		//std::cout << m_pWorld->m_Map[x][y].entities[0];
	}

	void WorldInterface::PlantBomb()
	{
		m_pIOManager->SendMsg(GetDelimiters(R"("event":"bomb")"));
	}

	// --------------------------------------------------------------------
	// Inbound requests
	// --------------------------------------------------------------------


	// --------------------------------------------------------------------
	WorldInterface::WorldInterface()
	{ }

	void WorldInterface::Init(BombermanGame& game)
	{
		m_pWorld = &game.m_World;

		// Send the central connection upstream
		Connect(game.GetIOManager());
		
		// Downcast syb::IBot to boom::IBot
		m_pBot = dynamic_cast<IBot*>(game.m_pBots[0]);
	}

	void WorldInterface::UpdateBot(const bool& update_map)
	{
		if (!m_pBotEntity)
			for (auto& entity : m_pWorld->m_Entities)
			{
				if (entity.second->Type() == IEntity::EntityType::PLAYER)
				{
					m_pBotEntity = std::dynamic_pointer_cast<Player>(entity.second);
					break;
				}
			}

		// This is called only after receiving the id and the first game_state from the server
		// i.e. everything is where it should be
		//m_pBot->x = (unsigned int)m_pWorld->m_Entities[m_pBot->my_id]->Position().x;
		//m_pBot->y = (unsigned int)m_pWorld->m_Entities[m_pBot->my_id]->Position().y;
		m_pBot->x = (unsigned int)m_pBotEntity->Position().x;
		m_pBot->y = (unsigned int)m_pBotEntity->Position().y;

		if (update_map)
		{
			unsigned int width = m_pWorld->m_pRules->MapWidth();
			unsigned int height = m_pWorld->m_pRules->MapHeight();
			
			if (m_pBot->world.empty())
			{
				m_pBot->world.resize(width);
				for (unsigned int h = 0; h != width; ++h)
					m_pBot->world[h].resize(height);
			}

			for (unsigned int x = 0; x < width; ++x)
			{
				for (unsigned int y = 0; y < height; ++y)
				{
					if (m_pWorld->m_Map[x][y].entities.size())
					{
						if (m_pWorld->m_Entities[m_pWorld->m_Map[x][y].entities[0]]->Type() == IEntity::EntityType::FIXBLOCK)
							m_pBot->world[x][y] = IBot::FIXLBOCK;
						else
							m_pBot->world[x][y] = IBot::WALKABLE;
					}
					else
						m_pBot->world[x][y] = IBot::EMPTY;
				}
			}
		}
	}

	std::string WorldInterface::GetDelimiters(const std::string& msg)
	{
		return "{" + msg + "}\n";
	}
} // namespace boom