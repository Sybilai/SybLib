#include "../include/World.hpp"
#include <utility>


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
	void Factory::CreateEntity(const Factory::PropertyBuffer& buffer, World& world)
	{
		// buffer is most likely the static Factory::buffer. Therefore, make a local copy. 
		PropertyBuffer buf = buffer;
		
		if (world.HasEntity(buf.object_id))
			return;

		// Bomberman has fixed integer coords snapped to a grid
		// i.e. no floats for us
		unsigned int pos_x = (unsigned int)buf.position.x;
		unsigned int pos_y = (unsigned int)buf.position.y;

		if (buf.type == "player")
		{
			//Player player(buf.object_id, IEntity::StringToType(buf.type), buf.position, Player::StringToDirection(buf.direction), world.m_pRules, buf.id);
			//std::unique_ptr<Player> p(&player);
			//world.m_Entities[buf.object_id] = std::move(p);
			std::shared_ptr<IEntity> player
				(
				new Player(buf.object_id, IEntity::StringToType(buf.type), buf.position, Player::StringToDirection(buf.direction), world.m_pRules, buf.id)
				);
			world.m_Entities.insert(std::pair<unsigned int, std::shared_ptr<IEntity>>(buf.object_id, player));
			world.m_Map[pos_x][pos_y].entities.push_back(buf.object_id); 
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "bomb")
		{
			//Bomb bomb(buf.object_id, IEntity::StringToType(buf.type), buf.position, world.m_pRules);
			//std::unique_ptr<Bomb> p(&bomb);
			//world.m_Entities[buf.object_id] = std::move(p);
			std::shared_ptr<IEntity> bomb(new Bomb(buf.object_id, IEntity::StringToType(buf.type), buf.position, world.m_pRules));
			world.m_Entities.insert(std::pair<unsigned int, std::shared_ptr<IEntity>>(buf.object_id, bomb));
			world.m_Map[pos_x][pos_y].entities.push_back(buf.object_id);
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "flame")
		{
			//Flame flame(buf.object_id, IEntity::StringToType(buf.type), buf.position, world.m_pRules);
			//std::unique_ptr<Flame> p(&flame);
			//world.m_Entities[buf.object_id] = std::move(p);
			std::shared_ptr<IEntity> flame(new Flame(buf.object_id, IEntity::StringToType(buf.type), buf.position, world.m_pRules));
			world.m_Entities.insert(std::pair<unsigned int, std::shared_ptr<IEntity>>(buf.object_id, flame));
			world.m_Map[pos_x][pos_y].entities.push_back(buf.object_id);
			world.ExpendKey(buf.object_id);
		}
		else if (buf.type == "fixblock")
		{
			//Block block(buf.object_id, IEntity::StringToType(buf.type), buf.position);
			//std::unique_ptr<FixBlock> p(&block);
			//world.m_Entities[buf.object_id] = std::move(p);
			//world.m_Entities[buf.object_id] = std::make_shared<IEntity>(block);
			std::shared_ptr<IEntity> block(new FixBlock(buf.object_id, IEntity::StringToType(buf.type), buf.position));
			world.m_Entities.insert(std::pair<unsigned int, std::shared_ptr<IEntity>>(buf.object_id, block));
			world.m_Map[pos_x][pos_y].entities.push_back(buf.object_id);
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
		m_NavGraph.Generate(*this);
	}
} // namespace boom