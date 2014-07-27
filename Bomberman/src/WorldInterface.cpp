#include "../include/sybil/networking/IOManager.hpp"
#include "../include/sybil/ai/Pathfinding.hpp"
#include "../include/WorldInterface.hpp"
#include <cstdlib>
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
		while (!m_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (IsMoveTargetValid(Target(x, y)))
		{
			// Not sure if should regenerate the whole graph. 
			// Consider considering.
			m_pWorld->GenerateNavGraph();

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
					{
						//directions.push_back("left");
						directions += R"("left",)";
						m_pBot->current_path.push_back(IBot::Direction::LEFT);
					}
					else
					{
						//directions.push_back("right");
						directions += R"("right",)";
						m_pBot->current_path.push_back(IBot::Direction::RIGHT);
					}
				}
				else
				{
					// On the same column(i.e. move... vertiways(?))
					if (from_y > to_y)
					{
						//directions.push_back("up");
						directions += R"("up",)";
						m_pBot->current_path.push_back(IBot::Direction::UP);
					}
					else
					{
						//directions.push_back("down");
						directions += R"("down",)";
						m_pBot->current_path.push_back(IBot::Direction::DOWN);
					}
				}
			}

			directions.erase(directions.end() - 1);
			directions += "]";
			m_pIOManager->SendMsg(GetDelimiters(directions));

			m_CurrentTarget.x = x;
			m_CurrentTarget.y = y;

			IsCurrentPathSafe();
		}
	}

	void WorldInterface::Stop()
	{
		std::string msg = R"("event":"move", "direction": [])";
		m_pIOManager->SendMsg(GetDelimiters(msg));
		m_pBot->current_path.erase(m_pBot->current_path.begin(), m_pBot->current_path.end());
	}

	void WorldInterface::PlantBomb()
	{
		while (!m_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		auto time = syb::Time::GetTime();

		if (IsBombValid(Target(m_pBot->x, m_pBot->y)))
		{
			if (syb::Time::FromSecTo(syb::Time::Elapsed(m_BombPlantTimeout, time), syb::Time::MILISEC) > 300)
			{
				if (!((m_CurrentTarget.x == m_pBot->x) && (m_CurrentTarget.y == m_pBot->y)))
					Stop();

				m_pIOManager->SendMsg(GetDelimiters(R"("event":"bomb")"));
				m_BombPlantTimeout = time;
			}
		}
	}

	bool WorldInterface::IsBombValid(const Target& target)
	{
		for (auto& bomb : m_pBot->bombs)
		{
			if ((bomb.x == target.x) && (bomb.y == target.y))
				return false;
		}
		return true;
	}

	bool WorldInterface::IsMoveTargetValid(const Target& target)
	{
		// Trying to move on a walled tile
		if (m_pBot->world[target.x][target.y] == IBot::FIXLBOCK)
			return false;

		// Already moving there
		if ((target.x == m_CurrentTarget.x) && (target.y == m_CurrentTarget.y))
			return false;

		// Already there
		if ((m_pBot->x == target.x) && (m_pBot->y == target.y))
			return false;

		return true;
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
					else if (m_pWorld->m_Entities[m_pWorld->m_Map[x][y].entities[0]]->Type() == IEntity::EntityType::FLAME)
						m_pBot->world[x][y] = IBot::FLAME;
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

	void WorldInterface::GetToSafety()
	{
	}

	bool WorldInterface::IsCurrentPathSafe()
	{
		// If the current_path starts like this:
		// A->B->C->D
		// When the bot is at B, the path will be the same:
		// A->B->C->D
		// When the target D is reached or the bot stops, the path is emptied:
		// NOTHING	
		unsigned int current_x = m_CurrentTarget.x,
					 current_y = m_CurrentTarget.y;
		
		if (m_pBot->world[current_x][current_y] == IBot::FLAME)
			return false;

		//for (auto it = m_pBot->current_path.end() - 1, end = m_pBot->current_path.begin(); it >= end; --it)
		for (int i = m_pBot->current_path.size() - 1; i >= 1; --i)
		{
			switch (m_pBot->current_path[i])
			{
			case IBot::Direction::UP:
				current_y--;
				break;
			case IBot::Direction::RIGHT:
				current_x++;
				break;
			case IBot::Direction::DOWN:
				current_y++;
				break;
			case IBot::Direction::LEFT:
				current_x--;
				break;
			}

			if (m_pBot->world[current_x][current_y] == IBot::FLAME)
				return false;
		}

		return true;
	}

	void WorldInterface::QueryBombs()
	{
		m_pBot->bombs.erase(m_pBot->bombs.begin(), m_pBot->bombs.end());
		m_pBot->bombs_within_range.erase(m_pBot->bombs_within_range.begin(), m_pBot->bombs_within_range.end());

		unsigned int bot_x = m_pBot->x,
					 bot_y = m_pBot->y,
					 range = m_pWorld->m_pRules->BombRange(),
					 bot_id = m_pBotEntity->Id();

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
		while (!m_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

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

		if (m_pBot->x == m_CurrentTarget.x && m_pBot->y == m_CurrentTarget.y)
			m_pBot->current_path.erase(m_pBot->current_path.begin(), m_pBot->current_path.end());

		if (update_map)
			QueryMap();

		QueryBombs();
		QueryPlayers();
	}

	int WorldInterface::GetClosestPlayer()
	{
		if (!m_pBot->players.size())
			return -1;

		// Avoid repeated dereferencing, and also copying 
		unsigned int bot_x = m_pBot->x;
		unsigned int bot_y = m_pBot->y;
		std::vector<IBot::Player>& players = m_pBot->players;

		// Manhattan distance: it's over ...
		unsigned int min_distance = 90001, distance = 0, min_player = 0;

		for (size_t it = 0; it < players.size(); ++it)
		{
			distance = abs((int)(players[it].x - bot_x)) + abs((int)(players[it].y - bot_y));
			if (distance < min_distance)
			{
				distance = min_distance;
				min_player = it;
			}
		}

		return min_player;
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
	WorldInterface::Target::Target(const unsigned int& x_, const unsigned int& y_) :
		x(x_), y(y_)
	{ }

	// --------------------------------------------------------------------
	std::string WorldInterface::GetDelimiters(const std::string& msg)
	{
		return "{" + msg + "}\n";
	}
} // namespace boom
