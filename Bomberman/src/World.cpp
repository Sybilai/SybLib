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

		if (buf.type == "player")
		{
			Player player(buf.id, buf.position, Player::StringToDirection(buf.direction), world.m_pRules);
			std::unique_ptr<Player> p(&player);
			world.m_Map[buf.position.x][buf.position.y].entities.push_back(std::move(p)); 
		}
		else if (buf.type == "bomb")
		{
			Bomb bomb(buf.id, buf.position, world.m_pRules);
			std::unique_ptr<Bomb> p(&bomb);
			world.m_Map[buf.position.x][buf.position.y].entities.push_back(std::move(p));
		}
		else if (buf.type == "flame")
		{
			Flame flame(buf.id, buf.position, world.m_pRules);
			std::unique_ptr<Flame> p(&flame);
			world.m_Map[buf.position.x][buf.position.y].entities.push_back(std::move(p));
		}
		else if (buf.type == "fixblock")
		{
			FixBlock block(buf.id, buf.position);
			std::unique_ptr<FixBlock> p(&block);
			world.m_Map[buf.position.x][buf.position.y].entities.push_back(std::move(p));
		}
	}

	World::World() :
		m_pRules(std::make_shared<GameRules>())
	{ }

	// --------------------------------------------------------------------
	bool World::HasEntity(const unsigned int& id)
	{
		if (id > m_ActiveKeys.size())
			return false;

		// Key is guaranteed to be either pooled or in use/active.
		return m_ActiveKeys[id];
	}

	// --------------------------------------------------------------------
	void World::GenerateNavGraph()
	{

	}
} // namespace boom