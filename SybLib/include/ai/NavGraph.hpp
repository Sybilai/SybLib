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
		Connection(const NodeId_t& from, const NodeId_t& to, const float& cost = 1.0f);
		Connection();

		NodeId_t m_FromNode;
		NodeId_t m_ToNode;
		float m_Cost;
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