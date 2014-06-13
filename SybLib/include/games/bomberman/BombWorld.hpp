#ifndef BOMB_WORLD_HPP
#define BOMB_WORLD_HPP

#include "utils/Vec2.hpp"
#include "Bomb.hpp"
#include "Flame.hpp"
#include "Bomberman.hpp"

#include <map>
#include <string>
#include <utility>


namespace syb
{
	class BombWorld
	{
	public:
		typedef std::map<int, Bomb> BombMap;
		typedef std::map<int, Bomberman> BombermanMap;
		typedef std::map<int, Flame> FlameMap;

		void CreateEntity(bool isBlocking,
						  bool mortal,
						  std::string type,
						  Vec2 pos,
						  std::string direction,
						  int id)
		{
			if 
				(type == "player")
			{
				Bomberman bomberman(pos, id, direction);
				m_Bombermen.insert(std::pair<int, Bomberman>(id, bomberman));
				//m_Bombermen[id] = bomberman;
			}
			else if 
				(type == "bomb")
			{
				Bomb bomb(pos, id);
				m_Bombs.insert(std::make_pair(id, bomb));
				//m_Bombs[id] = bomb;
			}
			else if 
				(type == "flame")
			{
				Flame flame (pos, id);
				m_Flames.insert(std::make_pair(id, flame));
				//m_Flames[id] = flame;
			}
		}

		void DestroyEntity(const int &id)
		{
			BombMap::iterator itBombs = m_Bombs.find(id);
			if (itBombs != m_Bombs.end())
			{
				m_Bombs.erase(id);
				return;
			}

			BombermanMap::iterator itFaggots = m_Bombermen.find(id);
			if (itBombs != m_Bombs.end())
			{
				m_Bombermen.erase(id);
				return;
			}

			FlameMap::iterator itflames = m_Flames.find(id);
			if (itBombs != m_Bombs.end())
			{
				m_Flames.erase(id);
				return;
			}
		}

		void MoveEntity(const int &id, const Vec2 &new_pos)
		{
			BombMap::iterator itBombs = m_Bombs.find(id);
			if (itBombs != m_Bombs.end())
			{
				itBombs->second.m_Pos = new_pos;
				//m_Bombs[id].m_Pos = new_pos;
				return;
			}

			BombermanMap::iterator itFaggots = m_Bombermen.find(id);
			if (itFaggots != m_Bombermen.end())
			{
				itFaggots->second.m_Pos = new_pos;
				//m_Bombermen[id].m_Pos = new_pos;
				return;
			}

			FlameMap::iterator itFlames = m_Flames.find(id);
			if (itFlames != m_Flames.end())
			{
				itFlames->second.m_Pos = new_pos;
				//m_Flames[id].m_Pos = new_pos;
				return;
			}
		}

		// no da-i 4 juma noaptea
		static BombMap m_Bombs;
		static BombermanMap m_Bombermen;
		static FlameMap m_Flames;

		enum Blocks
		{
			BL_OPEN, // 0
			BL_FIXBLOCK,
			BL_BOT,
			BL_FLAME,
			BL_BOMB
		};
		static int m_FlatMap[21][16];

		static void FlattenMap()
		{
			for (int i = 0; i < 22; ++i)
				for (int j = 0; j < 17; ++j)
					m_FlatMap[i][j] = 0;
		}

		static int m_Width;
		static int m_Height;
	};

	BombWorld::BombMap BombWorld::m_Bombs;
	BombWorld::BombermanMap BombWorld::m_Bombermen;
	BombWorld::FlameMap BombWorld::m_Flames;

	int BombWorld::m_Width;
	int BombWorld::m_Height;

	int BombWorld::m_FlatMap[21][16];
} // namespace syb

#endif // BOMB_WORLD_HPP