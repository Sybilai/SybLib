#include "games/bomberman/BombWorld.hpp"


namespace syb
{
	/*BombWorld::BombMap BombWorld::m_Bombs;
	BombWorld::BombermanMap BombWorld::m_Bombermen;
	BombWorld::FlameMap BombWorld::m_Flames;*/
	std::vector<IEntity*> BombWorld::m_vEntities;
	std::vector<Bomb> BombWorld::m_vBombs;
	std::vector<Bomberman> BombWorld::m_vBombermen;
	std::vector<Flame> BombWorld::m_vFlames;

	int BombWorld::m_Width;
	int BombWorld::m_Height;

	int BombWorld::m_FlatMap[21][16];

	void BombWorld::CreateEntity(bool isBlocking,
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
			m_vBombermen.push_back(bomberman);
			m_vEntities.push_back(&m_vBombermen[m_vBombermen.size() - 1]);
			//m_Bombermen.insert(std::make_pair(id, bomberman));
			//m_Bombermen.insert(std::pair<int, Bomberman>(id, bomberman));
			//m_Bombermen[id] = bomberman;
		}
		else if
			(type == "bomb")
		{
			Bomb bomb(pos, id);
			m_vBombs.push_back(bomb);
			m_vEntities.push_back(&m_vBombs[m_vBombs.size() - 1]);
			//m_Bombs.insert(std::make_pair(id, bomb));
			//m_Bombs[id] = bomb;
		}
		else if
			(type == "flame")
		{
			Flame flame(pos, id);
			m_vFlames.push_back(flame);
			m_vEntities.push_back(&m_vFlames[m_vFlames.size() - 1]);
			//m_Flames.insert(std::make_pair(id, flame));
			//m_Flames[id] = flame;
		}
	}

	void BombWorld::DestroyEntity(const int &id)
	{
		/*BombMap::iterator itBombs = m_Bombs.find(id);
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
		}*/

		/*std::vector<IEntity*>::iterator it = GetEntityById(id);
		if (it != m_vEntities.end())
		{
			delete *it;
			it = m_vEntities.erase(it);
		}*/

		/*for (std::vector<IEntity*>::iterator it = m_vEntities.begin(), end = m_vEntities.end(); it != end; ++it)
		{
			if ((*it)->m_Id == id)
			{
				delete *it;
				it = m_vEntities.erase(it);
				break;
			}
		}*/

		for (unsigned int it = 0; it < m_vEntities.size(); ++it)
		{
			if (m_vEntities[it]->m_Id == id)
			{
				delete m_vEntities[it];
				m_vEntities.erase(m_vEntities.begin() + it);
				break;
			}
		}
	}

	void BombWorld::MoveEntity(const int &id, const Vec2 &new_pos)
	{
		//BombMap::iterator itBombs = m_Bombs.find(id);
		//if (itBombs != m_Bombs.end())
		//{
		//	itBombs->second.m_Pos = new_pos;
		//	//m_Bombs[id].m_Pos = new_pos;
		//	return;
		//}

		//BombermanMap::iterator itFaggots = m_Bombermen.find(id);
		//if (itFaggots != m_Bombermen.end())
		//{
		//	itFaggots->second.m_Pos = new_pos;
		//	//m_Bombermen[id].m_Pos = new_pos;
		//	return;
		//}

		//FlameMap::iterator itFlames = m_Flames.find(id);
		//if (itFlames != m_Flames.end())
		//{
		//	itFlames->second.m_Pos = new_pos;
		//	//m_Flames[id].m_Pos = new_pos;
		//	return;
		//}

		/*std::vector<IEntity*>::iterator it = GetEntityById(id);

		if (it != m_vEntities.end())
			(*it)->m_Pos = new_pos;*/
		/*for (std::vector<IEntity*>::iterator it = m_vEntities.begin(), end = m_vEntities.end(); it != end; ++it)
		{
			if ((*it)->m_Id == id)
			{
				(*it)->m_Pos = new_pos;
				break;
			}
		}*/
		for (unsigned int it = 0; it < m_vEntities.size(); ++it)
		{
			if (m_vEntities[it]->m_Id == id)
			{
				m_vEntities[it]->m_Pos = new_pos;
				break;
			}
		}
	}

	std::vector<IEntity*>::iterator BombWorld::GetEntityById(const int &id)
	{
		for (std::vector<IEntity*>::iterator it = m_vEntities.begin(), end = m_vEntities.end(); it != end; ++it)
		{
			if ((*it)->m_Id == id)
				return it;
		}

		return m_vEntities.end();
	}

	void BombWorld::FlattenMap()
	{
		for (int i = 0; i < 22; ++i)
			for (int j = 0; j < 17; ++j)
				m_FlatMap[i][j] = 0;
	}
} // namespace syb