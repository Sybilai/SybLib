#include "../include/NavGraph.hpp"
#include "../include/World.hpp"


namespace boom
{
	NavGraph::NavGraph()
	{ }

	void NavGraph::Generate(World& world)
	{
		bool valid_node;
		// Query(not really) open tiles
		// Assumes that the map is walled in
		for (unsigned int x = 1; x <= world.m_pRules->MapWidth() - 2; ++x)
		{
			for (unsigned int y = 1; y <= world.m_pRules->MapHeight() - 2; ++y)
			{
				valid_node = false;

				if (world.m_Map[x][y].entities.size())
				{
					unsigned int id = world.m_Map[x][y].entities[0];
					if (world.m_Entities[id]->Type() != IEntity::EntityType::FIXBLOCK)
						valid_node = true;
				}
				else
					valid_node = true;

				if (valid_node)
				{

				}
			}
		}
	}
} // namespace boom