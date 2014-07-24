#include "../../include/ai/NavGraph.hpp"


namespace syb
{
	std::vector<Connection> NavGraph::GetConnections(NodeId_t from_node)
	{
		return m_Connections;
	}

	NavGraph::NavGraph()
	{ }

	Connection::Connection(const NodeId_t& from, const NodeId_t& to, const unsigned int& cost) :
		m_FromNode(from),
		m_ToNode(to),
		m_Cost(cost)
	{ }
} // namespace syb