#ifndef BOOM_WORLD_HPP
#define BOOM_WORLD_HPP
#include "sybil/utils/Vec2.hpp"
#include "GameRules.hpp"
#include "NavGraph.hpp"
#include "Entity.hpp"
#include <string>
#include <vector>
#include <memory>
#include <map>


namespace boom
{
	class World;

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
		static void CreateEntity(World&);

		/// Create an entity using the passed arg and send it to World
		static void CreateEntity(const PropertyBuffer&, World&);
	};

	class World
	{
		friend class WorldInterface;
	public:
		struct Tile
		{
			//std::vector<std::unique_ptr<IEntity>> entities;
			std::vector<unsigned int> entities;
		};

		World();

		bool HasEntity(const unsigned int& id);
		void ExpendKey(const unsigned int& id);

		/// The sybil NavGraph isn't bound to any layout that the map assumes, therefore, 
		/// generation should be different for every game.
		void GenerateNavGraph();

		std::shared_ptr<GameRules> m_pRules;
		std::vector<std::vector<Tile>> m_Map;
		//std::map<unsigned int, std::unique_ptr<IEntity>> m_Entities;
		std::map<unsigned int, std::shared_ptr<IEntity>> m_Entities;

	private:
		/// If some key isn't active then it's supposed to be in the pool. If it is not allocated, then it's neither active nor pooled. 
		std::vector<bool> m_ActiveKeys;
		NavGraph m_NavGraph;
	};
} // namespace boom

#endif // BOOM_WORLD_HPP