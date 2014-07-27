#ifndef SYB_WORLDINTERFACE_HPP
#define SYB_WORLDINTERFACE_HPP
#include "../networking/IOManager.hpp"
#include <memory>


namespace syb
{
	/// Game-specific functions that act as a proxy between AIs and the world.
	class WorldInterface
	{
	public:

	protected:
		WorldInterface();
		virtual ~WorldInterface();
		static void Connect(IOManager* pIO);
		//std::unique_ptr<IOManager> m_pIOManager;
		static IOManager* m_pIOManager;
	};

	inline WorldInterface::WorldInterface()
	{ }
	
	inline WorldInterface::~WorldInterface()
	{ }
	
	inline void WorldInterface::Connect(IOManager* pIO)
	{
		m_pIOManager = pIO;
	}
} // namespace syb

#endif // SYB_WORLDINTERFACE_HPP