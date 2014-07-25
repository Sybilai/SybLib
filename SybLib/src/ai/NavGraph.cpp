#include "../../include/ai/NavGraph.hpp"


namespace syb
{
	std::vector<Connection> NavGraph::GetConnections(NodeId_t from_node)
	{
		/*std::vector<Connection> connections;
		int edges = 0;
		for (auto it = m_Connections.begin(), end = m_Connections.end(); (it != end) || (edges != 4); ++it)
		{
			if (it->m_FromNode == from_node)
			{
				edges++;
				connections.push_back(*it);
			}
		}
		return m_Connections;*/

		return m_Connections[from_node];
	}

	NavGraph::NavGraph()
	{ }

	Connection::Connection(const NodeId_t& from, const NodeId_t& to, const float& cost) :
		m_FromNode(from),
		m_ToNode(to),
		m_Cost(cost)
	{ }

	Connection::Connection()
	{ }
} // namespace syb