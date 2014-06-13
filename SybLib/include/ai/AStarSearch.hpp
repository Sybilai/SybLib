#ifndef SYB_GRAPH_NODE_HPP
#define SYB_GRAPH_NODE_HPP

// --------------------------------------------
// Creds: http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
// --------------------------------------------

#include "utils/Vec2.hpp"
#include "core/SybDef.hpp"
#include "games/bomberman/BombWorld.hpp"

#include <string>
#include <cmath>
#include <queue>
#include <vector>
#include <cstdlib>

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
			m_CumCost += (dir == 8 ? (dir % 2 == 0 ? 10 : 14) : 10);;
		}

		int HeuristicCost(const Vec2 &target)
		{
			static int dispX = target.x - m_Pos.x;
			static int dispY = target.y - m_Pos.y;
			static int cost;

			// Available heuristics for calculating the estimate cost:
			// Manhattan: sum of the displacement in tiles vertically and horizontally
			//cost = abs(dispX) + abs(dispY);
			// Euclidian: straight line distance between two points
			cost = static_cast<int>(sqrt(dispX*dispX + dispY * dispY));
			// Euclidian Squared: avoids the sqrt in the euclidian distance for performance improvements
			//cost = dispX*dispX + dispY * dispY;
			return (cost);
		}

	private:
		Vec2 m_Pos;
		int m_AdjCost;
		int m_CumCost;
	};

	static bool operator<(const GraphNode &lhs, const GraphNode &rhs)
	{
		return lhs.GetAdjCost() > rhs.GetAdjCost();
	}

	// --------------------------------------------------------------------
	// Will provide the shortest distance on accessible terrain, without taking into account some gameplay elements: flames, bombs, bots
	// Returns false if no such path exists given the situation
	// --------------------------------------------------------------------
	std::string Search(const Vec2 &start, const Vec2 &target)
	{
		const int dir = 4;
		static int dx[dir] = { 1, 0, -1, 0 };
		static int dy[dir] = { 0, 1, 0, -1 };

		static std::priority_queue<GraphNode> pq[2];
		static int pqi = 0; 
		static GraphNode *n0, *m0;
		static int i, j, x, y, xdx, ydy;
		static char c;

		//int visitedNodes[BombWorld::m_Width][BombWorld::m_Height];
		//int openNodes[BombWorld::m_Width][BombWorld::m_Height];
		static int visitedNodes[21][16];
		static int openNodes[21][16];

		for (y = 0; y < 15; ++y)
		{
			for (x = 0; x < 20; ++x)
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
				std::string path = "";
				while (!(x == start.x && y == start.y))
				{
					j = dirMap[x][y];
					c = '0' + (j + dir / 2) % dir;
					path = c + path;
					x += dx[j];
					y += dy[j];
				}

				delete n0;
				while (!pq[pqi].empty()) pq[pqi].pop();
				return path;
			}
			
			for (i = 0; i < dir; i++)
			{
				xdx = x + dx[i]; ydy = y + dy[i];

				if (!(xdx<0 || xdx > 20 - 1 || ydy<0 || ydy > 15 - 1 || BombWorld::m_FlatMap[xdx][ydy] == 1 || visitedNodes[xdx][ydy] == 1))
				{
					m0 = new GraphNode(Vec2(xdx, ydy), n0->GetCumCost(), n0->GetAdjCost());
					m0->Advance(i);
					m0->SetAdjCost(target);

					if (openNodes[xdx][ydy] == 0)
					{
						openNodes[xdx][ydy] = m0->GetAdjCost();
						pq[pqi].push(*m0);
						dirMap[xdx][ydy] = (i + dir / 2) % dir;
					}
					else if (openNodes[xdx][ydy]>m0->GetAdjCost())
					{
						openNodes[xdx][ydy] = m0->GetAdjCost();
						dirMap[xdx][ydy] = (i + dir / 2) % dir;

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
		return "";
	}

	std::string JsonPath(std::string dirPath)
	{
		std::vector<std::string> dirs(4);
		dirs[0] = "\"right\"";
		dirs[1] = "\"down\"";
		dirs[2] = "\"left\"";
		dirs[3] = "\"up\"";

		std::string result;

		for (unsigned int i = 0; i < dirPath.size() - 1; ++i)
		{
			std::string thisisridiculous;
			thisisridiculous += dirPath[i];
			int way = std::stoi(thisisridiculous);
			result += dirs[way];
			result += ",";
		}
		if (dirPath.size() - 1 > 0)
		{
			std::string thisisridiculous;
			thisisridiculous += dirPath[dirPath.size() - 1];
			result += dirs[std::stoi(thisisridiculous)];
		}

		return result;
	}
} // namespace syb

#endif // SYB_GRAPH_NODE_HPP