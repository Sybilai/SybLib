#ifndef SYB_GRAPH_NODE_HPP
#define SYB_GRAPH_NODE_HPP

#include "utils/Vec2.hpp"


namespace syb
{
	// ----------------------------------------------------------------------------
	class SYB_API GraphNode
	{
	public:
		GraphNode() : 
			m_Index(INVALID_NODE_INDEX) 
		{ }

		GraphNode(int index) : 
			m_Index(index) 
		{ }
		
		virtual ~GraphNode() 
		{ }

		void SetIndex(int index) 
		{ 
			m_Index = index; 
		}

		const int &Index() const 
		{ 
			return m_Index; 
		}

	protected:
		int m_Index;
	};

	// ----------------------------------------------------------------------------
	template <typename TInfo = void*>
	class SYB_API NavGraphNode : public GraphNode
	{
	public:
		NavGraphNode() : 
			m_Info(TInfo()) 
		{ }
		
		NavGraphNode(int index, Vec2 position) : 
			GraphNode(index),
			m_Position(position),
			m_Info(TInfo())
		{ }
		
		virtual ~NavGraphNode() 
		{ }

		void SetPosition(Vec2 position) 
		{ 
			m_Position = position; 
		}

		const Vec2 &Position() const 
		{ 
			return m_Position; 
		}

		void SetInfo(TInfo info) 
		{ 
			m_Info = info; 
		}

		const TInfo &Info() const 
		{ 
			return m_Info; 
		}
	protected:
		Vec2 m_Position;
		TInfo m_Info;
	};

} // namespace syb

#endif // SYB_GRAPH_NODE_HPP