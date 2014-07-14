#include "../../include/world/WorldManager.hpp"


namespace syb
{
	// --------------------------------------------------------------------
	// EntityArchetype
	// --------------------------------------------------------------------
	void WorldManager::EntityArchetype::AddComponent(IComponent* component, const unsigned int& type)
	{
		m_Components[type] = component;
		m_ComponentTypes.push_back(type);
	}

	// --------------------------------------------------------------------
	// WorldManager
	// --------------------------------------------------------------------
	WorldManager::WorldManager(const size_t& initial_size) :
		m_ComponentTable(initial_size)
	{
		AllocPoolSlots(initial_size);
	}
	
	// --------------------------------------------------------------------
	int WorldManager::AddEntity(EntityArchetype& entity)
	{
		if (m_EntityPool.size())
		{
			unsigned int slot = m_EntityPool.top();
			m_EntityPool.pop();

			for (ComponentType_t type : entity.m_ComponentTypes)
			{
				// sensible sidenote: goddamn
				std::vector<IComponent*>* cast_container = static_cast<std::vector<IComponent*>*>(m_ComponentTable[type]);
				cast_container->at(slot) = entity.m_Components[type];
			}

			return slot;
		}
		else
		{
			AllocPoolSlots();
		}

		return -1;
	}

	// --------------------------------------------------------------------
	template <typename Component>
	IComponent* WorldManager::GetComponentByType(const IComponent::EntityId& id, const ComponentType_t& type)
	{
		std::vector<Component> cast_container = static_cast<Component>(m_ComponentTable[type]);
		return m_ComponentTable[type][id];
	}

	// --------------------------------------------------------------------
	void WorldManager::AllocPoolSlots(const size_t& slots)
	{
		// The '0' slot is reserved for active/inactive flags
		for (unsigned int i = 0; i <= slots; ++i)
			m_EntityPool.push(i);
	}
} // namespace syb