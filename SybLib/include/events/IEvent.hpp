#ifndef SYB_IEVENT_HPP
#define SYB_IEVENT_HPP

#include <string>


namespace syb
{
	class IEvent
	{
	public:
		//using EventType_t = unsigned int;
		// Slower lookups, but should avoid too many clashes.
		using EventType_t = std::string;

		IEvent(EventType_t);
		virtual ~IEvent();
		const EventType_t& Type() const;

	protected:
		EventType_t m_Type;
	};
} // namespace syb

#endif // SYB_IEVENT_HPP