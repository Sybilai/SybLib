#ifndef SYB_IENTITY_HPP
#define SYB_IENTITY_HPP

#include "utils/Vec2.hpp"


namespace syb
{
	class IEntity
	{
	public:
		//virtual void SetupInstance() = 0;
		Vec2 m_Pos;
		int m_Id;
	protected:
		IEntity(Vec2 pos, int id) :
			m_Pos(pos),
			m_Id(id)
		{ }
	};
} // namespace syb

#endif // SYB_IENTITY_HPP