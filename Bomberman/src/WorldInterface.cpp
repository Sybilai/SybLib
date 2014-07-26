#include "../include/sybil/networking/IOManager.hpp"
#include "../include/sybil/ai/Pathfinding.hpp"
#include "../include/WorldInterface.hpp"
#include <memory>
#include <map>


namespace boom
{
	World* WorldInterface::m_pWorld = nullptr;
	IBot* WorldInterface::m_pBot = nullptr;
	//unsigned int WorldInterface::m_BotId = -1;
	std::shared_ptr<Player> WorldInterface::m_pBotEntity = nullptr;
	WorldInterface::Target WorldInterface::m_CurrentTarget;
	syb::Time::Clock WorldInterface::m_BombPlantTimeout;

	// --------------------------------------------------------------------
	// Outbound requests
	// --------------------------------------------------------------------
	void WorldInterface::GoTo(const unsigned int& x, const unsigned int& y)
	{
		if (x != m_CurrentTarget.x && y != m_CurrentTarget.y)
		{
			std::vector<syb::Connection> path = 
				syb::Dijkstra::Search(&m_pWorld->m_NavGraph, NavGraph::GetNodeId(m_pBot->x, m_pBot->y), NavGraph::GetNodeId(x, y));

			//static std::map<unsigned int, std::string> directions = { { 0, "up" }, { 1, "left" }, { 2, "right" }, { 3, "down" } };

			//std::vector<std::string> directions;
			std::string directions = R"("event":"move", "direction": [)";

			for (auto tile = path.begin(), end = path.end(); tile != end; ++tile)
			{
				syb::NodeId_t from_node = tile->m_FromNode;
				syb::NodeId_t to_node = tile->m_ToNode;

				// TODO: Use a function to be consistent with GetNodeId()'s formula.
				// node_id = x * 1000 + y
				// node_id / 1000 = x
				// node_id % 1000 = y
				unsigned int from_x = from_node / 1000;
				unsigned int from_y = from_node % 1000;
				unsigned int to_x = to_node / 1000;
				unsigned int to_y = to_node % 1000;

				if (from_y == to_y)
				{
					// On the same line(i.e. move sideways)
					if (from_x > to_x)
						//directions.push_back("left");
						directions += R"("left",)";
					else
						//directions.push_back("right");
						directions += R"("right",)";
				}
				else
				{
					// On the same column(i.e. move... vertiways(?))
					if (from_y > to_y)
						//directions.push_back("up");
						directions += R"("up",)";
					else
						//directions.push_back("down");
						directions += R"("down",)";
				}
			}

			directions.erase(directions.end() - 1);
			directions += "]";
			m_pIOManager->SendMsg(GetDelimiters(directions));

			m_CurrentTarget.x = x;
			m_CurrentTarget.y = y;
		}
	}

	void WorldInterface::PlantBomb()
	{
		auto time = syb::Time::GetTime();

		if (syb::Time::FromSecTo(syb::Time::Elapsed(m_BombPlantTimeout, time), syb::Time::MILISEC) > 300)
		{
			m_pIOManager->SendMsg(GetDelimiters(R"("event":"bomb")"));
			m_BombPlantTimeout = time;
		}
	}

	// --------------------------------------------------------------------
	// Inbound requests. Currently, querying by bots is unnecessary, since the bots' "senses" are updated
	// frame-by-frame through UpdateBot()
	// --------------------------------------------------------------------
	void WorldInterface::QueryMap()
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

	void WorldInterface::QueryPlayers()
	{
		// Wasteful, consider considering
		m_pBot->players.erase(m_pBot->players.begin(), m_pBot->players.end());
		m_pBot->players_within_range.erase(m_pBot->players_within_range.begin(), m_pBot->players_within_range.end());

		unsigned int bot_x = m_pBot->x;
		unsigned int bot_y = m_pBot->y;
		unsigned int range = m_pWorld->m_pRules->BombRange();
		unsigned int bot_id = m_pBotEntity->Id();

		for (auto& key : m_pWorld->m_PlayerKeys)
		{
			if (m_pWorld->m_Entities[key]->Id() != bot_id)
			{
				syb::Vec2 position = m_pWorld->m_Entities[key]->Position();

				if (position.x == bot_x)
				{
					if ((bot_y - range <= position.y) && (bot_y + range >= position.y))
						m_pBot->players_within_range.push_back(IBot::Player(position.x, position.y));
				}
				else if (position.y == bot_y)
				{
					if ((bot_x - range <= position.x) && (bot_x + range >= position.x))
						m_pBot->players_within_range.push_back(IBot::Player(position.x, position.y));
				}

				m_pBot->players.push_back(IBot::Player(position.x, position.y));
			}
		}
	}

	void WorldInterface::QueryBombs()
	{
		m_pBot->bombs.erase(m_pBot->bombs.begin(), m_pBot->bombs.end());
		m_pBot->bombs_within_range.erase(m_pBot->bombs_within_range.begin(), m_pBot->bombs_within_range.end());

		unsigned int bot_x = m_pBot->x;
		unsigned int bot_y = m_pBot->y;
		unsigned int range = m_pWorld->m_pRules->BombRange();
		unsigned int bot_id = m_pBotEntity->Id();

		for (auto& key : m_pWorld->m_BombKeys)
		{
			if (m_pWorld->m_Entities[key]->Id() != bot_id)
			{
				syb::Vec2 position = m_pWorld->m_Entities[key]->Position();

				if (position.x == bot_x)
				{
					if ((bot_y - range <= position.y) && (bot_y + range >= position.y))
						m_pBot->bombs_within_range.push_back(IBot::Bomb(position.x, position.y));
				}
				else if (position.y == bot_y)
				{
					if ((bot_x - range <= position.x) && (bot_x + range >= position.x))
						m_pBot->bombs_within_range.push_back(IBot::Bomb(position.x, position.y));
				}

				m_pBot->bombs.push_back(IBot::Bomb(position.x, position.y));
			}
		}
	}

	void WorldInterface::UpdateBot(const bool& update_map)
	{
		if (!m_pBotEntity)
		{
			unsigned int bot_id = m_pBot->my_id;
			for (auto& entity : m_pWorld->m_PlayerKeys)
			{
				std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(m_pWorld->m_Entities[entity]);
				if (player->PlayerId() == bot_id)
				{
					m_pBotEntity = player;
					break;
				}
			}
		}

		m_pBot->x = (unsigned int)m_pBotEntity->Position().x;
		m_pBot->y = (unsigned int)m_pBotEntity->Position().y;

		if (update_map)
			QueryMap();

		QueryBombs();
		QueryPlayers();
	}

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

		// Start timeout
		m_BombPlantTimeout = syb::Time::GetTime();
	}

	// --------------------------------------------------------------------
	WorldInterface::Target::Target() :
		x(0), y(0)
	{ }

	// --------------------------------------------------------------------
	std::string WorldInterface::GetDelimiters(const std::string& msg)
	{
		return "{" + msg + "}\n";
	}
} // namespace boom
