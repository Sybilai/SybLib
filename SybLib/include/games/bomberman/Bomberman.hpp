#ifndef BOMB_ERMAN_HPP
#define BOMB_ERMAN_HPP

#include "core/IEntity.hpp"


namespace syb
{
	class Bomberman : public IEntity
	{
	public:
		static unsigned int SPEED;

		enum Direction
		{
			NONE,
			UP,
			RIGHT,
			DOWN,
			LEFT
		};

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

	unsigned int Bomberman::SPEED;
} // namespace syb

#endif // BOMB_ERMAN_HPP