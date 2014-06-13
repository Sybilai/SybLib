#ifndef SYB_GRAPH_ASTAR_HPP
#define SYB_GRAPH_ASTAR_HPP

#include <vector>
#include <list>

#include "ai/Graph.hpp"
#include "utils/IndexedPriorityQueue.hpp"
#include "utils/Vec2.hpp"


namespace syb
{
	template <typename TGraph>
	class AStarSearch
	{
	public:
		typedef typename TGraph::EdgeType Edge;

		AStarSearch(TGraph &graph, int source, int target) :
			m_Graph(graph),
			m_ShortestPathTree(graph.NumNodes()),
			m_SearchFrontier(graph.NumNodes()),
			m_GCosts(graph.NumNodes(), 0.0),
			m_FCosts(graph.NumNodes(), 0.0),
			m_Source(source),
			m_Target(target)
		{
			Search();
		}

		std::vector<const Edge*> GetSPT() const
		{
			return m_ShortestPathTree;
		}

		std::list<int> GetPathToTarget() const
		{
			std::list<int> path;
			
			if (m_Target < 0)
				return path;
			
			int node = m_Target;
			path.push_front(node);
			while ((node != m_Source) && (m_ShortestPathTree[node] != 0))
			{
				node = m_ShortestPathTree[node]->From();
				path.push_front(node);
			}
			
			return path;
		}

		double GetCostToTarget() const
		{
			return m_GCosts[m_Target];
		}
	private:
		const TGraph &m_Graph;
		std::vector<double> m_GCosts;
		std::vector<double> m_FCosts;
		std::vector<const Edge*> m_ShortestPathTree;
		std::vector<const Edge*> m_SearchFrontier;
		int m_Source;
		int m_Target;

		/// ----------------------------------------------------------------------------------
		/// AStar_Search()
		///
		/// @brief A variation of GraphSearches::Dijkstra_Search in which the an estimated distance between two points
		/// is taken into account when calculating the cost, effectively directing the search towards the goal 	
		/// ----------------------------------------------------------------------------------
		void AStar_Search()
		{
			// Priority queue that stores the destination nodes of the edges on the search frontier in order of increasing adjusted cost
			// Adjusted cost = Cumulative cost(see Dijkstra_Search) + Estimate(heuristic) cost
			PQ<double> pq(m_AdjCosts, m_Graph.NumNodes());
			pq.Insert(m_Source);
			while (!pq.Empty())
			{
				int nextClosestNode = pq.Pop();
				m_ShortestPathTree[nextClosestNode] = m_SearchFrontier[nextClosestNode];
				
				if (nextClosestNode == m_Target)
					return;
				
				TGraph::Iterator it(m_Graph, nextClosestNode);
				for (const Edge *pE = it.begin(); !it.end(); pE = it.next())
				{
					// Available heuristics for calculating the estimate cost estCost:
					// Manhattan: sum of the displacement in tiles vertically and horizontally
					// Euclidian: straight line distance between two points
					// Euclidian Squared: avoids the sqrt in the euclidian distance
					// ~currently defaults to the euclidian distance due to template issues
					//double estCost = THeuristic::CalculateCost(m_Graph, m_Target, pE->To());
					double estCost = GetCostEuclid(m_Graph, m_Target, pE->To());
					double cumCost = m_CumCosts[nextClosestNode] + pE->Cost();

					if (m_SearchFrontier[pE->To()] == NULL)
					{
						m_AdjCosts[pE->To()] = cumCost + estCost;
						m_CumCosts[pE->To()] = cumCost;
						pq.Insert(pe->To());
						m_SearchFrontier[pE->To()] = pE;
					}
					else if ((cumCost < m_CumCosts[pE->To()]) && (m_ShortestPathTree[pE->To()] == NULL))
					{
						m_AdjCosts[pE->To()] = cumCost + estCost;
						m_CumCosts[pE->To()] = cumCost;
						pq.ChangePriority(pE->To());
						m_SearchFrontier[pE->To()] = pE;
					}
				}
			}
		}

		double GetCostEuclid(const TGraph &graph, int node1, int node2)
		{
			return Vec2Distance(graph.GetNode(node1).Pos(), graph.GetNode(node2).Pos());
		}
	};
} // namespace syb

#endif // SYB_GRAPH_ASTAR_HPP