#ifndef SYB_GRAPH_EDGE_HPP
#define SYB_GRAPH_EDGE_HPP

#ifndef SYB_DEF_HPP
#include "core/SybDef.hpp"
#endif


namespace syb
{
	class GraphEdge
	{
	public:
		GraphEdge(int from, int to, double cost) :
			m_FromNode(from),
			m_ToNode(to),
			m_Cost(cost)
		{ }

		GraphEdge(int from, int to) :
			m_FromNode(from),
			m_ToNode(to),
			m_Cost(1.0)
		{ }

		GraphEdge() :
			m_FromNode(INVALID_NODE_INDEX),
			m_ToNode(INVALID_NODE_INDEX),
			m_Cost(1.0)
		{ }

		virtual ~GraphEdge()
		{ }

		const int &From() const
		{
			return m_FromNode;
		}

		void SetFrom(int node_index)
		{
			m_FromNode = node_index;
		}

		const int &To() const
		{
			return m_ToNode;
		}

		void SetTo(int node_index)
		{
			m_ToNode = node_index;
		}

		const double &Cost() const
		{
			return m_Cost;
		}

		void SetCost(double cost)
		{
			m_Cost = cost;
		}

		bool operator == (const GraphEdge &rhs)
		{
			return 
				rhs.m_FromNode == this->m_FromNode &&
				rhs.m_ToNode == this->m_ToNode &&
				rhs.m_Cost == this->m_Cost;
		}

		bool operator != (const GraphEdge &rhs)
		{
			return !(*this == rhs);
		}

	protected:
		int m_FromNode;
		int m_ToNode;
		double m_Cost;
	};
} // namespace syb

#endif // SYB_GRAPH_EDGE_HPP