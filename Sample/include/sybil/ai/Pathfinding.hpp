#ifndef SYB_ASTAR_HPP
#define SYB_ASTAR_HPP
#include "NavGraph.hpp"
#include <vector>


namespace syb
{
	struct NodeRecord
	{
		NodeRecord();
		NodeId_t node;
		Connection connection;
		float cost_so_far;
		float estimated_total_cost;
	};

	struct PathfindingList
	{
		PathfindingList();
		NodeRecord SmallestElement();
		bool HasNode(unsigned int node);
		NodeRecord Find(unsigned int node);
		size_t Length();

		PathfindingList& operator +=(const NodeRecord& rhs);
		PathfindingList& operator -=(const NodeRecord& rhs);

		std::vector<NodeRecord> m_Nodes;
	};

	/*class AStar
	{
	public:
		struct Heuristic
		{
			Heuristic(const NodeId_t& goal);
			NodeId_t goal_node;

			float GetEstimate(const NodeId_t& from);
		};

		static void Search(NavGraph*, const NodeId_t& start, const NodeId_t& end, Heuristic&);
	};*/

	class Dijkstra
	{
	public:
		static std::vector<Connection> Search(NavGraph*, const NodeId_t& start, const NodeId_t& goal);
	};
} // namespace syb

#endif // SYB_ASTAR_HPP