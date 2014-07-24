#include "../include/NavGraph.hpp"
#include "../include/World.hpp"
#include <queue>
//#include <set>
#include <unordered_set>


namespace boom
{
	NavGraph::NavGraph()
	{ }

	bool NavGraph::Walkable(World& world, const unsigned int& x, const unsigned int& y)
	{
		if (world.m_Map[x][y].entities.size())
		{
			unsigned int id = world.m_Map[x][y].entities[0];
			if (world.m_Entities[id]->Type() != IEntity::EntityType::FIXBLOCK)
				return true;
		}
		else
			return true;

		return false;
	}

	syb::NodeId_t NavGraph::GetNodeId(const unsigned int& x, const unsigned int& y)
	{
		// E.g.: For tile 13x5, the node that could be "placed" there has the id 13005
		return x * 1000 + y;
	}

	void NavGraph::Generate(World& world)
	{
		struct Coord
		{
			Coord(const int& x_ = -1, const int& y_ = -1) : 
				x(x_), 
				y(y_) 
			{ }
			
			int x, y;
		} valid;

		// Find a valid starting node, from which the entire mesh will be generated
		// This assumes that the map doesn't have closed spaces(i.e. any open tile
		// may be accessed from any other open tile).
		for (unsigned int x_ = 1; x_ <= world.m_pRules->MapWidth() - 2, valid.x == -1; ++x_)
			for (unsigned int y_ = 1; y_ <= world.m_pRules->MapHeight() - 2, valid.x == -1; ++y_)
				if (Walkable(world, x_, y_))
				{
					valid.x = x_;
					valid.y = y_;
				}

		if (valid.x != -1)
		{
			// Generates a directed graph: A->B, B->A
			std::queue<Coord> frontier;
			frontier.push(valid);
			//std::set<Coord> closed_tiles;
			//std::unordered_set<Coord> closed_tiles;
			std::unordered_set<unsigned int> closed;
			closed.insert(GetNodeId(valid.x, valid.y));

			int dir_x[4] {0, 1, 0, -1};
			int dir_y[4] {-1, 0, 1, 0};

			while (frontier.size())
			{
				Coord current = frontier.front();
				frontier.pop();

				syb::NodeId_t current_id = GetNodeId(current.x, current.y);
				for (int direction = 0; direction < 4; ++direction)
				{
					int x = current.x + dir_x[direction];
					int y = current.y + dir_y[direction];

					unsigned int target_id = GetNodeId(x, y);

					if (Walkable(world, x, y))
					{
						m_Connections.push_back(syb::Connection(current_id, target_id, 0));
						if (closed.find(target_id) == closed.end())
						{
							frontier.push(Coord(x, y));
							closed.insert(target_id);
						}
					}
				}
				//closed.insert(current_id);
			}
		}
	}
} // namespace boom