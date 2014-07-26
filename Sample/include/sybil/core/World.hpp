#ifndef SYB_WORLD_HPP
#define SYB_WORLD_HPP

#include "../ai/WorldInterface.hpp"


namespace syb
{
	class World
	{
	public:
		World();
		virtual ~World();

		WorldInterface* Interface() { return m_pWorldInterface; }

	protected:
		WorldInterface* m_pWorldInterface;
	};
} // namespace syb

#endif // SYB_WORLD_HPP