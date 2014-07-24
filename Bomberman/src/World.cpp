#include "../include/World.hpp"
#include <iostream>


namespace boom
{
	// --------------------------------------------------------------------
	Factory::PropertyBuffer Factory::buffer;

	Factory::PropertyBuffer::PropertyBuffer() :
		is_blocking(false),
		is_mortal(false),
		type(),
		position(),
		last_update(0),
		direction("none"),
		bombs(0),
		kills(0),
		object_id(0),
		id(0),
		name()
	{ }

	// --------------------------------------------------------------------
	void Factory::CreateEntity(World& world)
	{
		CreateEntity(Factory::buffer, world);
	}

	// --------------------------------------------------------------------
	void Factory::CreateEntity(const Factory::PropertyBuffer& buf, World& world)
	{
		if (world.HasEntity(buf.object_id))
			return;

		// Bomberman has fixed integer coords snapped to a grid
		// i.e. no floats for us
		unsigned int pos_x = buf.position.x;
		unsigned int pos_y = buf.position.y;

		if (buf.type == "player")
		{
			Player player(buf.object_id, buf.position, Player::StringToDirection(buf.direction), world.m_pRules);
			std::unique_ptr<Player> p(&player);
			//world.m_Map[pos_x][pos_y].entities.push_back(std::move(p)); 
			world.m_Entities[buf.object_id] = std::move(p);
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "bomb")
		{
			Bomb bomb(buf.object_id, buf.position, world.m_pRules);
			std::unique_ptr<Bomb> p(&bomb);
			//world.m_Map[pos_x][pos_y].entities.push_back(std::move(p));
			world.m_Entities[buf.object_id] = std::move(p);
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "flame")
		{
			Flame flame(buf.object_id, buf.position, world.m_pRules);
			std::unique_ptr<Flame> p(&flame);
			//world.m_Map[pos_x][pos_y].entities.push_back(std::move(p));
			world.m_Entities[buf.object_id] = std::move(p);
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "fixblock")
		{
			FixBlock block(buf.object_id, buf.position);
			std::unique_ptr<FixBlock> p(&block);
			//world.m_Map[pos_x][pos_y].entities.push_back(std::move(p));
			world.m_Entities[buf.object_id] = std::move(p);
			world.ExpendKey(buf.object_id);
		}
	}

	// --------------------------------------------------------------------
	World::World() :
		m_pRules(std::make_shared<GameRules>())
	{ }

	// --------------------------------------------------------------------
	bool World::HasEntity(const unsigned int& id)
	{
		if (id >= m_ActiveKeys.size())
			return false;

		// Key is guaranteed to be either pooled or in use/active.
		return m_ActiveKeys[id];
	}

	// --------------------------------------------------------------------
	void World::ExpendKey(const unsigned int& id)
	{
		if (id >= m_ActiveKeys.size())
			m_ActiveKeys.resize(id + 2);

		m_ActiveKeys[id] = true;
	}

	// --------------------------------------------------------------------
	void World::GenerateNavGraph()
	{

	}
} // namespace boom