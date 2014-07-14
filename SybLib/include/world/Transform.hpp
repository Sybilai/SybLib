#ifndef SYB_TRANSFORM_HPP
#define SYB_TRANSFORM_HPP

#include "IComponent.hpp"
#include "Vec2.hpp"


namespace syb
{
	class Transform : public IComponent
	{
		// Updating is handled either per-frame by a Movement component or forced by the WorldManager.
		// Currently, this should be done explicitly using the provided setters, but, should performance
		// issues pop up(this being often updated), direct access could be provided, hence cutting through 
		// function calls.
		// Currently, updates by a Movement component look kinda like this:
		// transform.SetPosition(transform.Position() += m_LinearVelocity * dt);
		// Therefore, ease off when it comes to Movement, for now. 
		friend class Movement;
		//friend class WorldManager;
	public:
		Transform(const WorldSharedPtr& pWorld);
		~Transform();

		const Vec2& Position() const;
		void SetPosition(const Vec2&);
		
		const float& Orientation() const;
		void SetOrientation(const float&);
	
	private:
		Vec2 m_Position;
		float m_Orientation;
	};
	
	// Constructor and destructor inlined to save wasted space by a separate .cpp
	inline Transform::Transform(const WorldSharedPtr& pWorld) :
		IComponent(pWorld)
	{ }

	inline Transform::~Transform()
	{ }

	inline const Vec2& Transform::Position() const
	{
		return m_Position;
	}

	inline void Transform::SetPosition(const Vec2& new_position)
	{
		m_Position = new_position;
	}

	inline const float& Transform::Orientation() const
	{
		return m_Orientation;
	}

	inline void Transform::SetOrientation(const float& new_orientation)
	{
		m_Orientation = new_orientation;
	}
} // namespace syb


#endif // SYB_TRANSFORM_HPP