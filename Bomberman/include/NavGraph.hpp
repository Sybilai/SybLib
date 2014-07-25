#ifndef BOOM_NAVGRAPH_HPP
#define BOOM_NAVGRAPH_HPP

#include "sybil/ai/NavGraph.hpp"


namespace boom
{
	//class World;

	class NavGraph : public syb::NavGraph
	{
		friend class World;
	public:
		NavGraph();
		void Generate(World&);
		static bool Walkable(World&, const unsigned int& x, const unsigned int& y);
		static syb::NodeId_t GetNodeId(const unsigned int& x, const unsigned int& y);
	};
}

#endif // BOOM_NAVGRAPH_HPP