#ifndef SYB_IEVENT_FILTER_HPP
#define SYB_IEVENT_FILTER_HPP

#include "IEventListener.hpp"
#include "IEvent.hpp"
#include <vector>
#include <map>

namespace syb
{
	// TODO: decide if event filter-er sounds sexi-er
	class IEventFilter
	{
	public:
		using ListenerSet_t = std::map<std::string, std::vector<IEventListener*>>;

		IEventFilter(ListenerSet_t*);
		virtual ~IEventFilter();

		virtual void Filter(IEvent*) = 0;
		/// Batch filtering. Currently, events are filtered and broadcasted as soon as they are received. 
		//virtual void Filter(const std::queue<IEvent*>&) = 0;;
	protected:
		ListenerSet_t *m_pListeners;
	};
} // namespace syb

#endif // SYB_IEVENT_FILTER_HPP