#include "../include/sybil/networking/IOManager.hpp"
#include "../include/sybil/ai/Pathfinding.hpp"
#include "../include/WorldInterface.hpp"
#include <cstdlib>
#include <memory>
#include <queue>
#include <map>


namespace boom
{
	World* WorldInterface::s_pWorld = nullptr;
	IBot* WorldInterface::s_pBot = nullptr;
	//unsigned int WorldInterface::m_BotId = -1;
	std::shared_ptr<Player> WorldInterface::s_pBotEntity = nullptr;
	WorldInterface::Target WorldInterface::s_CurrentTarget;
	syb::Time::Clock WorldInterface::s_BombPlantTimeout;

	// --------------------------------------------------------------------
	// Outbound requests
	// --------------------------------------------------------------------
	void WorldInterface::GoTo(const unsigned int& x, const unsigned int& y)
	{
		while (!s_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (IsMoveTargetValid(Target(x, y)))
		{
			// Not sure if should regenerate the whole graph. 
			// Consider considering.
			s_pWorld->GenerateNavGraph();

			std::vector<syb::Connection> path = 
				syb::Dijkstra::Search(&s_pWorld->m_NavGraph, NavGraph::GetNodeId(s_pBot->x, s_pBot->y), NavGraph::GetNodeId(x, y));

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
						s_pBot->current_path.push_back(IBot::Direction::LEFT);
					}
					else
					{
						//directions.push_back("right");
						directions += R"("right",)";
						s_pBot->current_path.push_back(IBot::Direction::RIGHT);
					}
				}
				else
				{
					// On the same column(i.e. move... vertiways(?))
					if (from_y > to_y)
					{
						//directions.push_back("up");
						directions += R"("up",)";
						s_pBot->current_path.push_back(IBot::Direction::UP);
					}
					else
					{
						//directions.push_back("down");
						directions += R"("down",)";
						s_pBot->current_path.push_back(IBot::Direction::DOWN);
					}
				}
			}

			directions.erase(directions.end() - 1);
			directions += "]";
			m_pIOManager->SendMsg(GetDelimiters(directions));

			s_CurrentTarget.x = x;
			s_CurrentTarget.y = y;

			//IsCurrentPathSafe();
		}
	}

	void WorldInterface::Stop()
	{
		std::string msg = R"("event":"move", "direction": [])";
		m_pIOManager->SendMsg(GetDelimiters(msg));
		s_pBot->current_path.erase(s_pBot->current_path.begin(), s_pBot->current_path.end());
	}

	void WorldInterface::PlantBomb()
	{
		while (!s_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		auto time = syb::Time::GetTime();

		if (IsBombValid(Target(s_pBot->x, s_pBot->y)))
		{
			if (syb::Time::FromSecTo(syb::Time::Elapsed(s_BombPlantTimeout, time), syb::Time::MILISEC) > 300)
			{
				if (!((s_CurrentTarget.x == s_pBot->x) && (s_CurrentTarget.y == s_pBot->y)))
					Stop();

				m_pIOManager->SendMsg(GetDelimiters(R"("event":"bomb")"));
				s_BombPlantTimeout = time;
			}
		}
	}

	bool WorldInterface::IsBombValid(const Target& target)
	{
		for (auto& bomb : s_pBot->bombs)
		{
			if ((bomb.x == target.x) && (bomb.y == target.y))
				return false;
		}
		return true;
	}

	bool WorldInterface::IsMoveTargetValid(const Target& target)
	{
		// Trying to move on a walled tile
		if (s_pBot->world[target.x][target.y] == IBot::FIXLBOCK)
			return false;

		// Already moving there
		if ((target.x == s_CurrentTarget.x) && (target.y == s_CurrentTarget.y))
			return false;

		// Already there
		if ((s_pBot->x == target.x) && (s_pBot->y == target.y))
			return false;

		return true;
	}

	// --------------------------------------------------------------------
	// Inbound requests. Currently, querying by bots is unnecessary, since the bots' "senses" are updated
	// frame-by-frame through UpdateBot()
	// --------------------------------------------------------------------
	void WorldInterface::QueryMap()
	{
		unsigned int width = s_pWorld->m_pRules->MapWidth();
		unsigned int height = s_pWorld->m_pRules->MapHeight();

		if (s_pBot->world.empty())
		{
			s_pBot->world.resize(width);
			for (unsigned int h = 0; h != width; ++h)
				s_pBot->world[h].resize(height);
		}

		for (unsigned int x = 0; x < width; ++x)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				if (s_pWorld->m_Map[x][y].entities.size())
				{
					if (s_pWorld->m_Entities[s_pWorld->m_Map[x][y].entities[0]]->Type() == IEntity::EntityType::FIXBLOCK)
						s_pBot->world[x][y] = IBot::FIXLBOCK;
					else if (s_pWorld->m_Entities[s_pWorld->m_Map[x][y].entities[0]]->Type() == IEntity::EntityType::FLAME)
						s_pBot->world[x][y] = IBot::FLAME;
					else
						s_pBot->world[x][y] = IBot::WALKABLE;
				}
				else
					s_pBot->world[x][y] = IBot::EMPTY;
			}
		}
	}

	void WorldInterface::QueryPlayers()
	{
		// Wasteful, consider considering
		s_pBot->players.erase(s_pBot->players.begin(), s_pBot->players.end());
		s_pBot->players_within_range.erase(s_pBot->players_within_range.begin(), s_pBot->players_within_range.end());

		unsigned int &bot_x = s_pBot->x,
					 &bot_y = s_pBot->y,
					 range = s_pWorld->m_pRules->BombRange(),
					 bot_id = s_pBotEntity->Id();

		for (auto& key : s_pWorld->m_PlayerKeys)
		{
			if (s_pWorld->m_Entities[key]->Id() != bot_id)
			{
				syb::Vec2 position = s_pWorld->m_Entities[key]->Position();

				if (position.x == bot_x)
				{
					if ((bot_y - range <= position.y) && (bot_y + range >= position.y))
						s_pBot->players_within_range.push_back(IBot::Player(position.x, position.y));
				}
				else if (position.y == bot_y)
				{
					if ((bot_x - range <= position.x) && (bot_x + range >= position.x))
						s_pBot->players_within_range.push_back(IBot::Player(position.x, position.y));
				}

				s_pBot->players.push_back(IBot::Player(position.x, position.y));
			}
		}
	}

	bool WorldInterface::TileHasBomb(const Target& target)
	{
		for (auto& entity : s_pWorld->m_Map[target.x][target.y].entities)
		{
			if (s_pWorld->m_Entities.find(entity) != s_pWorld->m_Entities.end())
			//if (s_pWorld->m_Entities[entity])
				if (s_pWorld->m_Entities[entity]->Type() == IEntity::EntityType::BOMB)
					return true;
		}

		return false;
	}

	bool WorldInterface::IsTileSafe(const unsigned int& tile_x, const unsigned int& tile_y)
	{
		unsigned int x, 
					 y,
					 bomb_range = s_pWorld->m_pRules->BombRange() + 2;
		
		std::vector<Target> potential_bombs;
		std::vector<std::vector<unsigned int>>& world = s_pBot->world;

		// Baaad. To change.
		y = tile_y;
		for (x = tile_x; x >= tile_x - bomb_range; --x)
		{
			if (world[x][y] == IBot::FIXLBOCK)
				break;
			if (TileHasBomb(Target(x, y)))
				return false;
		}

		for (x = tile_x + 1; x <= tile_x + bomb_range; ++x)
		{
			if (world[x][y] == IBot::FIXLBOCK) 
				break;
			if (TileHasBomb(Target(x, y)))
				return false;
		}

		x = tile_x;
		for (y = tile_y; y >= tile_y - bomb_range; --y)
		{
			if (world[x][y] == IBot::FIXLBOCK)
				break;
			if (TileHasBomb(Target(x, y)))
				return false;
		}

		for (y = tile_y + 1; y <= tile_y + bomb_range; ++y)
		{
			if (world[x][y] == IBot::FIXLBOCK)
				break;
			if (TileHasBomb(Target(x, y)))
				return false;
		}

		return true;
	}

	void WorldInterface::GetToSafety()
	{
		std::vector<std::vector<unsigned int>> world = s_pBot->world;
		
		unsigned int &bot_x = s_pBot->x,
					 &bot_y = s_pBot->y,
					 bomb_range = s_pWorld->m_pRules->BombRange(); // returns a const ref

		unsigned int x, y;

		// Mark all tiles that are about to be bound by flame. 
		for (auto& bomb : s_pBot->bombs)
		{
			y = bomb.y;
			for (x = bomb.x - bomb_range; x <= bomb.x + bomb_range; ++x)
			{
				if (world[x][y] != IBot::FIXLBOCK)
					world[x][y] = IBot::FLAME;
			}

			x = bomb.x;
			for (y = bomb.y - bomb_range; y <= bomb.y + bomb_range; ++y)
			{
				if (world[x][y] != IBot::FIXLBOCK)
					world[x][y] = IBot::FLAME;
			}
		}

		int dx[4] { 0, 1, 0, -1},
			dy[4] {-1, 0, 1,  0};

		std::queue<Target> frontier;
		frontier.push(Target(bot_x, bot_y));

		Target closest(0, 0);

		// Find the closest tile that is safe.
		// Looks bad.
		while (world.size() && ((closest.x == 0) && (closest.y == 0)))
		//while (world[frontier.front().x][frontier.front().y] == IBot::FLAME)
		{
			Target current(frontier.front().x, frontier.front().y);
			frontier.pop();

			for (unsigned int i = 0; i < 4; ++i)
			{
				unsigned int current_x = current.x + dx[i],
							 current_y = current.y + dy[i];
				
				if (world[current_x][current_y] == IBot::EMPTY)
				{
					closest.x = current_x;
					closest.y = current_y;
					break;
				}
				else if (world[current_x][current_y] == IBot::FLAME)
				{
					frontier.push(Target(current_x, current_y));
				}
			}
		}

		GoTo(closest.x, closest.y);
	}

	bool WorldInterface::IsCurrentPathSafe()
	{
		// If the current_path starts like this:
		// A->B->C->D
		// When the bot is at B, the path will be the same:
		// A->B->C->D
		// When the target D is reached or the bot stops, the path is emptied:
		// NOTHING	
		unsigned int current_x = s_CurrentTarget.x,
					 current_y = s_CurrentTarget.y;
		
		if (s_pBot->world[current_x][current_y] == IBot::FLAME)
			return false;
		//for (auto it = s_pBot->current_path.end() - 1, end = s_pBot->current_path.begin(); it >= end; --it)
		for (int i = s_pBot->current_path.size() - 1; i >= 1; --i)
		{
			switch (s_pBot->current_path[i])
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

			if (s_pBot->world[current_x][current_y] == IBot::FLAME)
				return false;
		}

		return true;
	}

	void WorldInterface::QueryBombs()
	{
		s_pBot->bombs.erase(s_pBot->bombs.begin(), s_pBot->bombs.end());
		s_pBot->bombs_within_range.erase(s_pBot->bombs_within_range.begin(), s_pBot->bombs_within_range.end());

		unsigned int bot_x = s_pBot->x,
					 bot_y = s_pBot->y,
					 range = s_pWorld->m_pRules->BombRange(),
					 bot_id = s_pBotEntity->Id();

		for (auto& key : s_pWorld->m_BombKeys)
		{
			if (s_pWorld->m_Entities[key]->Id() != bot_id)
			{
				syb::Vec2 position = s_pWorld->m_Entities[key]->Position();

				if (position.x == bot_x)
				{
					if ((bot_y - range <= position.y) && (bot_y + range >= position.y))
						s_pBot->bombs_within_range.push_back(IBot::Bomb(position.x, position.y));
				}
				else if (position.y == bot_y)
				{
					if ((bot_x - range <= position.x) && (bot_x + range >= position.x))
						s_pBot->bombs_within_range.push_back(IBot::Bomb(position.x, position.y));
				}

				s_pBot->bombs.push_back(IBot::Bomb(position.x, position.y));
			}
		}
	}

	void WorldInterface::UpdateBot(const bool& update_map)
	{
		while (!s_pBot)
		{
			// VM thread started before the game finished initialising.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (!s_pBotEntity)
		{
			unsigned int bot_id = s_pBot->my_id;
			for (auto& entity : s_pWorld->m_PlayerKeys)
			{
				std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(s_pWorld->m_Entities[entity]);
				if (player->PlayerId() == bot_id)
				{
					s_pBotEntity = player;
					break;
				}
			}
		}

		s_pBot->x = (unsigned int)s_pBotEntity->Position().x;
		s_pBot->y = (unsigned int)s_pBotEntity->Position().y;

		if (s_pBot->x == s_CurrentTarget.x && s_pBot->y == s_CurrentTarget.y)
			s_pBot->current_path.erase(s_pBot->current_path.begin(), s_pBot->current_path.end());

		if (update_map)
			QueryMap();

		QueryBombs();
		QueryPlayers();
	}

	int WorldInterface::GetClosestPlayer()
	{
		if (!s_pBot->players.size())
			return -1;

		// Avoid repeated dereferencing, and also copying 
		unsigned int bot_x = s_pBot->x;
		unsigned int bot_y = s_pBot->y;
		std::vector<IBot::Player>& players = s_pBot->players;

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
		s_pWorld = &game.m_World;

		// Send the central connection upstream
		Connect(game.GetIOManager());
		
		// Downcast syb::IBot to boom::IBot
		s_pBot = dynamic_cast<IBot*>(game.m_pBots[0]);

		// Start timeout
		s_BombPlantTimeout = syb::Time::GetTime();
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
