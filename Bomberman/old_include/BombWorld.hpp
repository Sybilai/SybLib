#ifndef BOMB_WORLD_HPP
#define BOMB_WORLD_HPP

#include "utils/Vec2.hpp"
#include "BombEntities.hpp"

#include <map>
#include <string>
#include <vector>
#include <utility>


namespace syb
{
	class BombWorld
	{
	public:
		/*typedef std::map<int, Bomb> BombMap;
		typedef std::map<int, Bomberman> BombermanMap;
		typedef std::map<int, Flame> FlameMap;*/

		void CreateEntity(bool isBlocking,
						  bool mortal,
						  std::string type,
						  Vec2 pos,
						  std::string direction,
						  int id);

		void DestroyEntity(const int &id);

		void MoveEntity(const int &id, const Vec2 &new_pos);

		std::vector<IEntity*>::iterator GetEntityById(const int &id);

		void FlattenMap();
		
		// no da-i 4 juma noaptea
		/*static BombMap m_Bombs;
		static BombermanMap m_Bombermen;
		static FlameMap m_Flames;*/
		/*std::vector<IEntity*> m_vEntities;
		std::vector<Bomb> m_vBombs;
		std::vector<Bomberman> m_vBombermen;
		std::vector<Flame> m_vFlames;*/
		static std::vector<IEntity*> m_vEntities;
		static std::vector<Bomb> m_vBombs;
		static std::vector<Bomberman> m_vBombermen;
		static std::vector<Flame> m_vFlames;

		enum Block
		{
			BL_OPEN, // 0
			BL_FIXBLOCK,
			BL_BOT,
			BL_FLAME,
			BL_BOMB
		};

		struct Tile
		{
			std::vector<Block> m_Blocks;
		};

		static int m_FlatMap[21][16];
		static Tile m_Map[21][16];

		static int m_Width;
		static int m_Height;
	};
} // namespace syb

#endif // BOMB_WORLD_HPP