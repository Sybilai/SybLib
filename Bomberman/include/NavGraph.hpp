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
	};
}

#endif // BOOM_NAVGRAPH_HPP