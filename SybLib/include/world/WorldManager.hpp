#ifndef SYB_WORLD_MANAGER_HPP
#define SYB_WORLD_MANAGER_HPP

#include "IComponent.hpp"
#include <unordered_map>
#include <vector>
#include <stack>
#include <map>


namespace syb
{
	/**
	 * Entity lifecycle: 
	 * 
	 * auth data = stuff that says something about stuff in the game.
	 * specific event = event that affects the number of entities(e.g. 'create', 'destroy', etc.).
	 *
	 *            
	 *              auth                    post                     specific
	 * #Authority ----------> #IOManager -----------> #EventManager ---------> #CustomFactory   ----
	 *              data                    event                     event                        | AddEntity
	 *																							   |-------------> #WorldManager 
	 *                                                                                             | RemoveEntity
	 *                                                                         #EntityArchetype ----
	 *
	 */
	class WorldManager
	{
	public:
		using ComponentType_t = unsigned int;

		class EntityArchetype
		{
			friend class WorldManager;

			std::map<ComponentType_t, IComponent*> m_Components;
			std::vector<ComponentType_t> m_ComponentTypes;
		public:
			/// Sugar
			/// Since the archetype "blueprint" is supposed to be known by the factory, removing is not that necessary.
			void AddComponent(IComponent*, const unsigned int& type);
		};

		/// "Standard" component types
		/// Derived games should use values from 15 upward(allow for the base lib to extend without clashes in the future).
		enum ComponentType
		{
			C_TRANSFORM,
			C_MOVEMENT
		};

		/// Allocates enough(in theory) memory so that frequent allocations are avoided early on, giant allocations
		/// later, and lots of copying in general.
		WorldManager(const size_t& initial_size = DEFAULT_ALLOC_SIZE);

		/// Returns the id assigned to the added @entity.
		int AddEntity(EntityArchetype& entity);

		template <typename Component>
		IComponent* GetComponentByType(const IComponent::EntityId& id, const ComponentType_t& type);

	private:
		static const size_t DEFAULT_ALLOC_SIZE = 128;
		void AllocPoolSlots(const size_t& slots = DEFAULT_ALLOC_SIZE);
		
		/// Table "format":
		/// m_ComponentTable[ComponentType][Slot]
		/// The number of component types is given by a game, accounting for the "standard"s and its own.
		///
		/// Each slot represents an entity: 
		/// Gigel is composed of/represented by: 
		///				m_ComponentTable[1][myGigel.id], m_ComponentTable[4][myGigel.id], m_ComponentTable[6][myGigel.id]
		/// When myGigel is destroyed, myGigel.id goes to the pool and made available for new entities to come into 
		/// this brave new world.
		//std::unordered_map<ComponentType, std::vector<IComponent*>> m_ComponentTable;
		std::unordered_map<ComponentType_t, void*> m_ComponentTable;
		// sensible suggestion: rename to my_Slots so it sounds all naughty
		std::vector<bool> m_Slots;
		std::stack<unsigned int> m_EntityPool;
	};
} // namespace syb

#endif // SYB_WORLD_MANAGER_HPP