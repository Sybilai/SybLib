#ifndef SYB_NAVGRAPH_HPP
#define SYB_NAVGRAPH_HPP
#include <vector>
#include <list>


namespace syb
{
	using NodeId_t = unsigned int;

	/// Performance critical. Avoid accessors or some other fancy shtick.
	class Connection
	{
	public:
		Connection(const NodeId_t& from, const NodeId_t& to, const unsigned int& cost);

		NodeId_t from_node;
		NodeId_t to_node;
		unsigned int cost;
	};
	
	class NodeRecord
	{
	public:
		NodeId_t node;
		Connection connection;
		unsigned int cost_so_far;
		unsigned int estimated_total_cost;
	};

	class PathfindingList
	{
	public:
		NodeRecord SmallestElement();
		bool HasNode(unsigned int node);
		NodeRecord Find(unsigned int node);

		PathfindingList& operator +=(const PathfindingList& rhs);
		PathfindingList& operator -=(const PathfindingList& rhs);
	};

	/// To be generated on a per-project basis
	class NavGraph
	{
	public:
		std::vector<Connection> GetConnections(NodeId_t from_node);

	protected:
		NavGraph();
		std::vector<Connection> m_Connections;
	};
} // namespace syb

#endif // SYB_NAVGRAPH_HPP