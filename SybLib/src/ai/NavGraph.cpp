#include "../../include/ai/NavGraph.hpp"


namespace syb
{
	std::vector<Connection> NavGraph::GetConnections(NodeId_t from_node)
	{
		return m_Connections;
	}

	NavGraph::NavGraph()
	{

	}
} // namespace syb