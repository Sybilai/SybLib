#ifndef BOMB_BOMB_HPP
#define BOMB_BOMB_HPP

#include "core/IEntity.hpp"


namespace syb
{
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

	unsigned int Bomb::LIFE;
	unsigned int Bomb::RANGE;
	unsigned int Bomb::SPEED;
} // namespace syb

#endif // BOMB_BOMB_HPP