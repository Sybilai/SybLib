#ifndef SYB_GRAPH_HPP
#define SYB_GRAPH_HPP

#include <vector>
#include <list>

#ifndef SYB_DEF_HPP
#include "core/SybDef.hpp"
#endif

namespace syb
{
	template <typename TNode, typename TEdge>
	class Graph
	{
	public:
		typedef std::vector<TNode> NodeVector;
		typedef std::list<TEdge> EdgeList;
		typedef std::vector<EdgeList> EdgeListVector;
	
		Graph(bool digraph = false) :
			m_NextNodeIndex(0),
			m_bDigraph(digraph)
		{ }

		const TNode &GetNode(int index) const
		{
			return m_Nodex[index];
		}

		TNode &GetNode(int index)
		{
			return m_Nodes[index];
		}

		const TEdge &GetEdge(int from, int to) const
		{
			for (EdgeList::const_iterator it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
				if (it->To() == to)
					return *it;
		}

		TEdge &GetEdge(int from, int to) const
		{
			for (EdgeList::iterator it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
				if (it->To() == to)
					return *it;
		}

		const int NextFreeNodeIndex() const
		{
			return m_NextNodeIndex;
		}

		int AddNode(TNode node)
		{
			if (node.Index() < m_Nodes.size())
			{
				m_Nodes[node.Index()] = node;
				return m_NextNodeIndex;
			}

			m_Nodes.push_back(node);
			m_Edges.push_back(EdgeList());
			return m_NextNodeIndex;
		}

		void RemoveNode(int node)
		{
			m_Nodes[node].SetIndex(INVALID_NODE_INDEX);

			if (!m_bDigraph)
			{
				for (EdgeList::iterator itEdge = m_Edges[node].begin(), endEdge = m_Edges[node].end(); itEdge != endEdge; ++itEdge)
				{
					for (EdgeList::iterator it = m_Edges[itEdge->To()].begin(), end = m_Edges[itEdge->To()].end(); it != end; ++it)
						if (it->To() == node)
						{
							it = m_Edges[itEdge->To()].erase(it);
							break;
						}
				}

				m_Edges[node].clear();
			}
			else
			{
				RemoveInvalidEdges();
			}
		}

		void AddEdge(TEdge edge)
		{
			if ((m_Nodes[edge.To()].Index() != INVALID_NODE_INDEX) && (m_Nodes[edge.From()].Index() != INVALID_NODE_INDEX))
			{
				if (UniqueEdge(edge.From(), edge.To()))
					m_Edges[edge.From()].push_back(edge);
				if (!m_bDigraph)
					if (UniqueEdge(edge.To(), edge.From()))
					{
						TEdge newEdge = edge;
						newEdge.SetTo(edge.From());
						newEdge.SetFrom(edge.To());
						m_Edges[edge.To()].push_back(newEdge);
					}
			}
		}

		void RemoveEdge(int from, int to)
		{
			EdgeList::iterator it, end;

			if (!m_bDigraph)
			{
				for (it = m_Edges[to].begin(), end = m_Edges[to].end(); it != end; ++it)
					if (it->To() == from)
					{
						it = m_Edges[to].erase(it);
						break;
					}
			}

			for (it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
				if (it->To() == to)
				{
					it = m_Edges[from].erase(it); 
					break;
				}
		}

		void SetEdgeCost(int from, int to, double cost)
		{
			for (EdgeList::iterator it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
				if (it->To() == to)
				{
					it->SetCost(cost);
					break;
				}
		}

		const int &NumNodes() const
		{
			return m_Nodes.size();
		}

		const int &NumActiveNodes() const
		{
			int count = 0;
			for (unsigned int i = 0; i < m_Nodes.size(); ++i)
				if (m_Nodes[i].Index() != INVALID_NODE_INDEX)
					++count;
			return count;
		}

		const int &NumEdges() const
		{
			int count = 0;
			for (EdgeListVector::const_iterator it = m_Edges.begin(), end = m_Edges.end(); it != end; ++it)
				count += it->size();
			return count;
		}

		bool IsDigraph() const
		{
			return m_bDigraph;
		}

		bool IsEmpty() const
		{
			return m_Nodex.empty();
		}

		bool HasNode(int node_index) const
		{
			if (node_index >= m_Nodes.size() || (m_Nodes[node_index].Index() == INVALID_NODE_INDEX))
				return false;
			return true;
		}

		bool HasEdge(int from, int to) const
		{
			if (HasNode(from) && HasNode(to))
			{
				for (EdgeList::const_iterator it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
					if (it->To() == to)
						return true;
			}
			return false;
		}

		void Clear()
		{
			m_NextNodeIndex = 0;
			m_Nodes.clear();
			m_Edges.clear();
		}

		void RemoveEdges()
		{
			for (EdgeListVector::iterator it = m_Edges.begin(), end = m_Edges.end(); it != end; ++it)
				it->clear();
		}

		class EdgeIterator
		{
		public:
			EdgeIterator(Graph<TNode, TEdge> &graph, int node) :
				m_Graph(graph),
				m_NodeIndex(node)
			{
				m_EdgePointer = m_Graph.m_Edges[m_NodeIndex].begin();
			}

			TEdge *begin()
			{
				m_EdgePointer = m_Graph.m_Edges[m_NodeIndex].begin();		
				return &(*m_EdgePointer);
			}

			TEdge *next()
			{
				++m_EdgePointer;
				if (end())
					return NULL;
				return &(*m_EdgePointer);
			}

			bool end()
			{
				return (m_EdgePointer == m_Graph.m_Edges[m_NodeIndex].end());
			}
		private:
			typename EdgeList::iterator m_EdgePointer;
			Graph<TNode, TEdge> &m_Graph;
			const int m_NodeIndex;
		};

		friend class EdgeIterator;

		class ConstEdgeIterator
		{
		public:
			ConstEdgeIterator(const Graph<TNode, TEdge> &graph, int node) :
				m_Graph(graph),
				m_NodeIndex(node)
			{
				m_EdgePointer = m_Graph.m_Edges[m_NodeIndex].begin();
			}

			const TEdge *begin()
			{
				m_EdgePointer = m_Graph.m_Edges[NodeIndex].begin();
				if (End())
					return 0;
				return &(*m_EdgePointer);
			}

			const TEdge *next()
			{
				++m_EdgePointer;
				if (end())
					return NULL;			
				return &(*m_EdgePointer);
			}

			bool end()
			{
				return (m_EdgePointer == m_Graph.m_Edges[m_NodeIndex].end());
			}
		private:
			typename EdgeList::const_iterator m_EdgePointer;
			Graph<TNode, TEdge> &m_Graph;
			const int m_NodeIndex;
		};

		friend class ConstEdgeIterator;

		class NodeIterator
		{
		public:
			NodeIterator(Graph<TNode, TEdge> &graph) :
				m_Graph(graph)
			{
				m_NodePointer = m_Graph.m_Nodes.begin();
			}

			TNode* begin()
			{
				m_NodePointer = m_Graph.m_Nodes.begin();
				GetNextValidNode(m_NodePointer);
				return &(*m_NodePointer);
			}

			TNode* next()
			{
				++m_NodePointer;
				if (end()) 
					return NULL;
				GetNextValidNode(m_NodePointer);
				return &(*m_NodePointer);
			}

			bool end()
			{
				return (m_NodePointer == m_Graph.m_Nodes.end());
			}
		private:
			typename NodeVector::iterator m_NodePointer;
			Graph<TNode, TEdge> &m_Graph;
			
			void GetNextValidNode(typename NodeVector::iterator &it)
			{
				if (m_NodePointer == m_Graph.m_Nodes.end() || it->Index() != INVALID_NODE_INDEX)
					return;

				while (it->Index() == INVALID_NODE_INDEX)
				{
					++it;
					if (m_NodePointer == m_Graph.m_Nodes.end())
						break;
				}
			}
		};

		friend class NodeIterator;

		class ConstNodeIterator
		{
		public:
			ConstNodeIterator(const Graph<TNode, TEdge> &graph) :
				m_Graph(graph)
			{
				m_NodePointer = m_Graph.m_Nodes.begin();
			}

			const TNode* begin()
			{
				m_NodePointer = m_Graph.m_Nodes.begin();
				GetNextValidNode(m_NodePointer);
				return &(*m_NodePointer);
			}

			const TNode* next()
			{
				++m_NodePointer;
				if (end())
					return NULL;
				GetNextValidNode(m_NodePointer);
				return &(*m_NodePointer);
			}

			bool end()
			{
				return (m_NodePointer == m_Graph.m_Nodes.end());
			}
		private:
			typename NodeVector::const_iterator m_NodePointer;
			Graph<TNode, TEdge> &m_Graph;

			void GetNextValidNode(typename NodeVector::const_iterator &it)
			{
				if (m_NodePointer == m_Graph.m_Nodes.end() || it->Index() != INVALID_NODE_INDEX)
					return;

				while (it->Index() == INVALID_NODE_INDEX)
				{
					++it;
					if (m_NodePointer == m_Graph.m_Nodes.end())
						break;
				}
			}
		};

		friend class ConstNodeIterator;
	private:
		NodeVector m_Nodes;
		EdgeListVector m_Edges;
		bool m_bDigraph;
		int m_NextNodeIndex;

		bool UniqueEdge(int from, int to) const
		{
			for (EdgeList::const_iterator it = m_Edges[from].begin(), end = m_Edges[from].end(); it != end; ++it)
				if (it->To() == to)
					return false;
			return true;
		}

		void RemoveInvalidEdges()
		{
			for (EdgeListVector::iterator itList = m_Edges.begin(), endList = m_Edges.end(); itList != endList; ++itList)
				for (EdgeList::iterator it = itList->begin(), end = itList->end(); it != end; ++it)
					if (m_Nodes[it->To()].Index() == INVALID_NODE_INDEX || m_Nodes[it->From()].Index() == INVALID_NODE_INDEX)
						it = itList->erase(it);
		}
	};
} // namespace syb

#endif // SYB_GRAPH_HPP