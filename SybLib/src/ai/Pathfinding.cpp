#include "../../include/ai/Pathfinding.hpp"
#include <vector>


namespace syb
{
	NodeRecord::NodeRecord() :
		cost_so_far(0.0f),
		estimated_total_cost(0.0f)
	{ }

	AStar::Heuristic::Heuristic(const NodeId_t& goal) :
		goal_node(goal)
	{ }

	float AStar::Heuristic::GetEstimate(const NodeId_t& from)
	{
		return 0.0f;
	}

	NodeRecord PathfindingList::SmallestElement()
	{
		NodeRecord min;
		min.cost_so_far = 9000.1f;

		for (auto record : m_Nodes)
			if (record.cost_so_far < min.cost_so_far)
				min = record;

		return min;
	}

	bool PathfindingList::HasNode(unsigned int node)
	{
		for (auto record : m_Nodes)
			if (record.node == node)
				return true;

		return false;
	}

	NodeRecord PathfindingList::Find(unsigned int node)
	{
		NodeRecord default_record;

		for (auto record : m_Nodes)
			if (record.node == node)
				return record;

		return default_record;
	}

	PathfindingList& PathfindingList::operator +=(const NodeRecord& rhs)
	{
		m_Nodes.push_back(rhs);
		return *this;
	}

	PathfindingList& PathfindingList::operator -=(const NodeRecord& rhs)
	{
		for (auto it = m_Nodes.begin(), end = m_Nodes.end(); it != end; ++it)
			if (it->node == rhs.node)
				m_Nodes.erase(it);

		return *this;
	}

	size_t PathfindingList::Length()
	{
		return m_Nodes.size();
	}

	void AStar::Search(NavGraph* pGraph, const NodeId_t& start, const NodeId_t& end, Heuristic& heuristic)
	{
		NodeRecord start_record;
		start_record.node = start;
		start_record.cost_so_far = 0;
		start_record.estimated_total_cost = heuristic.GetEstimate(start);

		PathfindingList open, closed;
		open += start_record;

		while (open.Length())
		{
			NodeRecord current = open.SmallestElement();

			if (current.node == end)
				break;
			else
			{
				auto connections = pGraph->GetConnections(current.node);

				for (auto connection : connections)
				{
					unsigned int end_node = connection.m_ToNode;
					float end_node_cost = current.cost_so_far + connection.m_Cost;

					if (closed.HasNode(end_node))
					{
						NodeRecord end_node_record = closed.Find(end_node);

						if (end_node_record.cost_so_far <= end_node_cost)
							continue;

						closed -= end_node_record;

						//float end_node_heuristic = end_node_record.
					}
				}
			}
		}
	}

	std::vector<Connection> Dijkstra::Search(NavGraph* pGraph, const NodeId_t& start, const NodeId_t& goal)
	{
		NodeRecord start_record;
		start_record.node = start;
		start_record.cost_so_far = 0;

		PathfindingList open, closed;
		open += start_record;

		NodeRecord current;

		while (open.Length())
		{
			current = open.SmallestElement();
			
			if (current.node == goal)
				break;

			auto connections = pGraph->GetConnections(current.node);

			for (auto& connection : connections)
			{
				NodeRecord end_node_record;
				NodeId_t end_node = connection.m_ToNode;
				float end_node_cost = current.cost_so_far + connection.m_Cost;

				if (closed.HasNode(end_node))
					continue;
				else if (open.HasNode(end_node))
				{
					end_node_record = open.Find(end_node);
					if (end_node_record.cost_so_far <= end_node_cost)
						continue;
				}
				else
				{
					end_node_record.node = end_node;
				}

				end_node_record.cost_so_far = end_node_cost;
				end_node_record.connection = connection;

				if (!open.HasNode(end_node))
					open += end_node_record;
			}

			open -= current;
			closed += current;	
		}
		
		std::vector<Connection> path;

		if (current.node != goal)
		{
			// nope
		}
		else
		{
			while (current.node != start)
			{
				path.push_back(current.connection);
				current.node = current.connection.m_FromNode;
			}

			std::reverse(path.begin(), path.end());
		}

		return path;
	}
} // namespace syb