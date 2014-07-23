#ifndef SYB_ICOMPONENT_HPP
#define SYB_ICOMPONENT_HPP

#include <memory>


namespace syb
{
	class WorldManager;

	class IComponent
	{
	public:	
		using EntityId = unsigned int;
		using ComponentId = unsigned int;
		using WorldSharedPtr = std::shared_ptr<WorldManager>;
		
		IComponent(const WorldSharedPtr& pWorld) :
			m_pWorld(pWorld)
		{ }


	protected:
		// Needed for dynamic casting
		virtual void MakeMePolymorphicAndStuff() 
		{ }
		
		WorldSharedPtr m_pWorld;
		EntityId m_ParentId;
	
	};
} // namespace syb

#endif // SYB_ICOMPONENT_HPP