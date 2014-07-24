#include "../include/World.hpp"


namespace boom
{
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

	void Factory::CreateEntity()
	{

	}

	void Factory::CreateEntity(const Factory::PropertyBuffer& buf)
	{

	}

	bool World::HasEntity(const unsigned int& id)
	{
		if (id > m_ActiveKeys.size())
			return false;

		// Key is guaranteed to be either pooled or in use/active.
		return m_ActiveKeys[id];
	}
} // namespace boom