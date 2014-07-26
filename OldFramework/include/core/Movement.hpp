#ifndef SYB_MOVEMENT_HPP
#define SYB_MOVEMENT_HPP

#include "IComponent.hpp"
#include "Transform.hpp"
#include "../utils/Vec2.hpp"


namespace syb
{
	/**
	 * Currently, only holds a velocity component for some entities which can be either
	 * 0 or some value taken from the authorit(in something like game_rules). 
	 *
	 * Since in the Sybil universe, stuff has a profound distaste for anything that Newton dudebro
	 * conceived, things change from their current state to their desired state(i.e. pos + orient)
	 * in an instant(-ish). 
	 *
	 * Steering is for mortals, anyway. 
	 */
	class Movement : public IComponent
	{
	public:
		Movement(const WorldSharedPtr& pWorld, Transform *pTransform = nullptr);
		Movement(const WorldSharedPtr& pWorld, const float& linear_x, const float& linear_y, Transform *pTransform = nullptr, const float& angular = 0.0f);
		Movement(const WorldSharedPtr& pWorld, const Vec2& linear_velocity, Transform *pTransform = nullptr, const float& angular = 0.0f);
		~Movement();

		/// Updates the Transform component that shares a common parent entity with this. 
		void UpdateEntity(const float& dt);

		/// Updates the Transform component of an entity given by its @id.
		void UpdateEntity(const EntityId&, const float& dt);

		/// Updates a given Transform component(which presumably shares a common enity with this).
		/// This is supposed to be slightly prettier than UpdateEntity() and UpdateEntity(const EntityId&)
		/// since there aren't any lookups/queries in the WorldManager performed. 
		/// i.e. use mostly this
		void UpdateEntity(Transform&, const float& dt);

		const Vec2& LinearVelocity() const;
		void SetLinearVelocity(const Vec2&);

		const float& AngularVelocity() const;
		void SetAngularVelocity(const float&);

	private:
		Transform* m_pTransform;

		/// Speed of the entity in each axis. 
		Vec2 m_LinearVelocity;
		
		/// The number of rads per unit of time that the orientation of the entity chages.
		/// TODO: unit = either seconds or frames, depending on update policy in Game::
		/// Currently inactive, see header comment, the one where I'm all funny and stuff. 
		float m_AngularVelocity;
	};

	inline const Vec2& Movement::LinearVelocity() const
	{
		return m_LinearVelocity;
	}

	inline void Movement::SetLinearVelocity(const Vec2& new_velocity)
	{
		m_LinearVelocity = new_velocity;
	}

	inline const float& Movement::AngularVelocity() const
	{
		return m_AngularVelocity;
	}

	inline void Movement::SetAngularVelocity(const float& new_velocity)
	{
		m_AngularVelocity = new_velocity;
	}
} // namespace syb

#endif // SYB_MOVEMENT_HPP