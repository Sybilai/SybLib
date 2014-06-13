#ifndef SYB_GRAPH_NODE_HPP
#define SYB_GRAPH_NODE_HPP

#include "utils/Vec2.hpp"
#include "core/SybDef.hpp"
#include "games/bomberman/BombWorld.hpp"

#include <string>
#include <cmath>
#include <queue>

namespace syb
{
	class GraphNode
	{
	public:
		GraphNode(Vec2 pos, int adj_cost, int cum_cost) : 
			m_Pos(pos),
			m_AdjCost(adj_cost),
			m_CumCost(cum_cost)
		{ }
		
		virtual ~GraphNode() { }

		const Vec2 &GetPos() const { return m_Pos; }
		const int &GetAdjCost() const { return m_AdjCost; }
		const int &GetCumCost() const { return m_CumCost; }

		void SetAdjCost(const Vec2 &target)
		{
			// Weighted A* - focus on states with low heuristic values
			// Use 0 as weight for both costs to default to Dijkstra
			m_AdjCost = m_CumCost * 1 + HeuristicCost(target) * 10;
		}

		void Advance(const int &dir)
		{
			m_CumCost += 10;
		}

		int HeuristicCost(const Vec2 &target)
		{
			int dispX = target.x - m_Pos.x;
			int dispY = target.y - m_Pos.y;
			int cost;

			// Available heuristics for calculating the estimate cost:
			// Manhattan: sum of the displacement in tiles vertically and horizontally
			//cost = abs(dispX) + abs(dispY);
			// Euclidian: straight line distance between two points
			cost = (int)(sqrt(dispX*dispX + dispY * dispY));
			// Euclidian Squared: avoids the sqrt in the euclidian distance for performance improvements
			//cost = dispX*dispX + dispY * dispY;
			return cost;
		}

		static bool operator<(const GraphNode &lhs, const GraphNode &rhs)
		{
			return lhs.GetAdjCost() > rhs.GetAdjCost();
		}

	private:
		Vec2 m_Pos;
		int m_AdjCost;
		int m_CumCost;
	};

	// --------------------------------------------------------------------
	// Will provide the shortest distance on accessible terrain, without taking into account some gameplay elements: flames, bombs, bots
	// Returns false if no such path exists given the situation
	// --------------------------------------------------------------------
	bool Search(const Vec2 &start, const Vec2 &target, std::string &path)
	{
		std::priority_queue<GraphNode> pq[2];
		int pqi = 0; 
		GraphNode *n0, *m0;

		//int visitedNodes[BombWorld::m_Width][BombWorld::m_Height];
		//int openNodes[BombWorld::m_Width][BombWorld::m_Height];
		int visitedNodes[21][16];
		int openNodes[21][16];

		int x, y;

		for (y = 0; y < BombWorld::m_Height; ++y)
		{
			for (x = 0; x < BombWorld::m_Width; ++x)
			{
				visitedNodes[x][y] = 0;
				openNodes[x][y] = 0;
			}
		}

		n0 = new GraphNode(start, 0, 0);
		n0->SetAdjCost(target);
		pq[pqi].push(*n0);
		openNodes[x][y] = n0->GetAdjCost();

		int dirMap[21][16];
		int dx[4] = { 1, 0, -1, 0 };
		int dy[4] = { 0, 1, 0, -1 };

		while (!pq[pqi].empty())
		{
			n0 = new GraphNode(Vec2(pq[pqi].top().GetPos().x, pq[pqi].top().GetPos().y), pq[pqi].top().GetCumCost(), pq[pqi].top().GetAdjCost());

			x = n0->GetPos().x;
			y = n0->GetPos().y;

			pq[pqi].pop();
			openNodes[x][y] = 0;
			visitedNodes[x][y] = 1;

			if (x == target.x && y == target.y)
			{
				while (!(x == start.x && y == start.y))
				{
					int j = dirMap[x][y];
					char c = '0' + (j + 2) % 4;
					path = c + path;
					x += dx[j];
					y += dy[j];
				}

				delete n0;
				while (!pq[pqi].empty()) pq[pqi].pop();
				return true;
			}
			
			int xdx, ydy;

			for (int i = 0; i< 4; i++)
			{
				xdx = x + dx[i]; ydy = y + dy[i];

				if (!(xdx<0 || xdx>BombWorld::m_Width - 1 || ydy<0 || ydy>BombWorld::m_Height - 1 || BombWorld::m_FlatMap[xdx][ydy] == 1
					|| visitedNodes[xdx][ydy] == 1))
				{
					m0 = new GraphNode(Vec2(xdx, ydy), n0->GetCumCost(), n0->GetAdjCost());
					m0->Advance(i);
					m0->SetAdjCost(target);

					if (openNodes[xdx][ydy] == 0)
					{
						openNodes[xdx][ydy] = m0->GetAdjCost();
						pq[pqi].push(*m0);
						dirMap[xdx][ydy] = (i + 4 / 2) % 4;
					}
					else if (openNodes[xdx][ydy]>m0->GetAdjCost())
					{
						openNodes[xdx][ydy] = m0->GetAdjCost();
						dirMap[xdx][ydy] = (i + 4 / 2) % 4;

						while (!(pq[pqi].top().GetPos().x == xdx &&
							pq[pqi].top().GetPos().y == ydy))
						{
							pq[1 - pqi].push(pq[pqi].top());
							pq[pqi].pop();
						}
						pq[pqi].pop();

						if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
						while (!pq[pqi].empty())
						{
							pq[1 - pqi].push(pq[pqi].top());
							pq[pqi].pop();
						}
						pqi = 1 - pqi;
						pq[pqi].push(*m0);
					}
					else delete m0;
				}
			}
			delete n0;
		}
		return false;
	}
} // namespace syb

#endif // SYB_GRAPH_NODE_HPP