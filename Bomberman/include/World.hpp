#ifndef BOOM_WORLD_HPP
#define BOOM_WORLD_HPP
#include "sybil/utils/Vec2.hpp"
#include <string>
#include <vector>


namespace boom
{
	class Factory
	{
		friend class BombermanGame;

		/// New event descriptor. Entity agnostic. Allocated once and used for every entity that needs the factory's services.
		struct PropertyBuffer
		{
			// Unnecessary. Consider slashing.
			PropertyBuffer();

			bool is_blocking;
			bool is_mortal;
			std::string type;
			syb::Vec2 position;
			//powerups
			unsigned int last_update;
			std::string direction;
			unsigned int bombs;
			unsigned int kills;
			unsigned int object_id;
			unsigned int id; // ffs dude fo real
			std::string name;
		};

		static PropertyBuffer buffer;

	public:
		/// Create an entity using buffer and send it to World
		static void CreateEntity();

		/// Create an entity using the passed arg and send it to World
		static void CreateEntity(const PropertyBuffer&);
	};

	class World
	{
	public:
		bool HasEntity(const unsigned int& id);

	private:
		/// If some key isn't active then it's supposed to be in the pool. If it is not allocated, then it's neither active nor pooled. 
		std::vector<bool> m_ActiveKeys;
	};
} // namespace syb

#endif // BOOM_WORLD_HPP