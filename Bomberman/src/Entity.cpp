#include "../include/Entity.hpp"


namespace boom
{
	//IEntity::m_ConversionTable["player"] = IEntity::EntityType::PLAYER;
	std::map<std::string, IEntity::EntityType> IEntity::m_ConversionTable =
	{
		{ "player", IEntity::EntityType::PLAYER },
		{ "bomb", IEntity::EntityType::BOMB },
		{ "flame", IEntity::EntityType::FLAME },
		{ "fixblock", IEntity::EntityType::FIXBLOCK }
	};
} // namespace boom