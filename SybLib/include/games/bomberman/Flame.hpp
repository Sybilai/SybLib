#ifndef BOMB_FLAME_HPP
#define BOMB_FLAME_HPP

#include "core/IEntity.hpp"


namespace syb
{
	class Flame : public IEntity
	{
	public:
		static unsigned int LIFE;

		Flame(Vec2 pos, int id) :
			IEntity(pos, id)
		{ }
	};

	unsigned int Flame::LIFE;
} // namespace syb

#endif // BOMB_FLAME_HPP