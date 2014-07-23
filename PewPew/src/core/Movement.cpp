#include "../../include/core/Movement.hpp"
#include "../../include/core/WorldManager.hpp"


namespace syb
{
	Movement::Movement(const std::shared_ptr<WorldManager> &pWorld, Transform *pTransform) :
		IComponent(pWorld),
		m_LinearVelocity(0.0f, 0.0f),
		m_AngularVelocity(0.0f),
		m_pTransform(pTransform)
	{ }

	Movement::Movement(const WorldSharedPtr& pWorld, const float& linear_x, const float& linear_y, Transform *pTransform, const float& angular) :
		IComponent(pWorld),
		m_LinearVelocity(linear_x, linear_y),
		m_AngularVelocity(angular),
		m_pTransform(pTransform)
	{ }

	Movement::Movement(const WorldSharedPtr& pWorld, const Vec2& linear_velocity, Transform *pTransform, const float& angular) :
		IComponent(pWorld),
		m_LinearVelocity(linear_velocity),
		m_AngularVelocity(angular),
		m_pTransform(pTransform)
	{ }

	Movement::~Movement()
	{ }

	void Movement::UpdateEntity(const float& dt)
	{
		if (!m_pTransform)
			m_pTransform = dynamic_cast<Transform*>(m_pWorld->GetComponentByType<Transform>(m_ParentId, WorldManager::C_TRANSFORM));

		m_pTransform->m_Position += m_LinearVelocity * dt;
		// steering and orientation 
	}

	void Movement::UpdateEntity(const EntityId& id, const float& dt)
	{
		if (!m_pTransform)
			m_pTransform = dynamic_cast<Transform*>(m_pWorld->GetComponentByType<Transform>(id, WorldManager::C_TRANSFORM));

		m_pTransform->m_Position += m_LinearVelocity * dt;

		// steering and orientation
	}

	void Movement::UpdateEntity(Transform& transform, const float& dt)
	{
		// Newton-Euler Integration
		// sensible sidenote: Woah! Sounds SUPER smart!
		//transform.SetPosition(transform.Position() += m_LinearVelocity * dt);
		transform.m_Position += m_LinearVelocity * dt;

		// Currently on hold til further notice
		//transform.m_Orientation += m_AngularVelocity * dt;
		// TODO: implement steering: 
		//		 update angular and linear velocities based on a given steering structure
		// e.g. m_LinearVelocity += steering.linear * dt;
		//      m_AngularVelocity += steering.angular * dt;
	}
} // namespace syb