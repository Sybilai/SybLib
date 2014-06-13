#ifndef BOMB_ERMAN_HPP
#define BOMB_ERMAN_HPP

#include "core/IEntity.hpp"
#include "core/SybDef.hpp"

#include <string>


namespace syb
{
	class Bomberman : public IEntity
	{
	public:
		static unsigned int SPEED;

		Bomberman(Vec2 pos, int id, std::string direction) :
			IEntity(pos, id)
		{ 
			if (direction == "none")
				m_Direction = NONE;
			else if (direction == "up")
				m_Direction = UP;
			else if (direction == "right")
				m_Direction = RIGHT;
			else if (direction == "down")
				m_Direction = DOWN;
			else if (direction == "left")
				m_Direction = LEFT;
		}

		Direction m_Direction;
	};

	class Bomb : public IEntity
	{
	public:
		static unsigned int LIFE;
		static unsigned int RANGE;
		static unsigned int SPEED;

		Bomb(Vec2 pos, int id) :
			IEntity(pos, id)
		{ }
	};

	class Flame : public IEntity
	{
	public:
		static unsigned int LIFE;

		Flame(Vec2 pos, int id) :
			IEntity(pos, id)
		{ }
	};
} // namespace syb

#endif // BOMB_ERMAN_HPP