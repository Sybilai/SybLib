#include "../../include/events/EventManager.hpp"


namespace syb
{
	// --------------------------------------------------------------------
	// EventManager
	// --------------------------------------------------------------------
	EventManager::EventManager()
	{ }

	EventManager::~EventManager()
	{ }

	void EventManager::Init()
	{
		//EventManager *instance = new EventManager;
		//return instance;
	}

	void EventManager::RegisterFilter(IEventFilter* filter)
	{
		m_pGameFilter = filter;
	}

	void EventManager::RegisterListener(IEventListener* listener)
	{
		m_Listeners[listener->Type()].push_back(listener);
	}

	void EventManager::Lock()
	{
		m_Mutex.lock();
	}

	void EventManager::Unlock()
	{
		m_Mutex.unlock();
	}
} // namespace syb