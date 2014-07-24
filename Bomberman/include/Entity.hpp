#ifndef BOOM_ENTITY_HPP
#define BOOM_ENTITY_HPP
#include "sybil/utils/Vec2.hpp"
#include "GameRules.hpp"
#include <memory>
#include <string>
#include <map>


namespace boom
{
	class IEntity
	{
	public:
		enum class EntityType
		{
			PLAYER, 
			BOMB,
			FLAME,
			FIXBLOCK
		};
		static EntityType StringToType(const std::string&);

	private:
		syb::Vec2 m_Position;
		unsigned int m_EntityId;
		EntityType m_Type;
		static std::map<std::string, EntityType> m_ConversionTable;

	public:
		const syb::Vec2& Position() const;
		void SetPosition(const syb::Vec2& position);
		const unsigned int& Id() const;
		const EntityType& Type() const;

	protected:
		IEntity(const unsigned int& id, const EntityType&, const syb::Vec2& position);
	};

	class IRuledEntity
	{
	protected:
		IRuledEntity(const std::shared_ptr<GameRules>&);
		std::shared_ptr<GameRules> m_pRules;
	};

	class Player : public IEntity, public IRuledEntity
	{
	public:
		enum class PlayerDirection
		{
			NONE,
			UP,
			RIGHT,
			DOWN,
			LEFT
		};
		static PlayerDirection StringToDirection(const std::string&);

		Player(
			const unsigned int& id, 
			const EntityType&,
			const syb::Vec2& position, 
			const PlayerDirection&, 
			const std::shared_ptr<GameRules>&, 
			const unsigned int& player_id);

		const PlayerDirection& Direction() const;
		void SetDirection(const PlayerDirection&);

		const unsigned int& PlayerId() const;
		const unsigned int& Speed() const;

	private:
		PlayerDirection m_Direction;
		unsigned int m_PlayerId;
	};

	class FixBlock : public IEntity
	{
	public:
		FixBlock(const unsigned int& id, const EntityType&, const syb::Vec2& position);
	};

	class Bomb : public IEntity, public IRuledEntity
	{
	public:
		Bomb(const unsigned int& id, const EntityType&, const syb::Vec2& position, const std::shared_ptr<GameRules>&);

		const unsigned int& Life() const;
		const unsigned int& Range() const;
		const unsigned int& Speed() const;
	};

	class Flame : public IEntity, public IRuledEntity
	{
	public:
		Flame(const unsigned int& id, const EntityType&, const syb::Vec2& position, const std::shared_ptr<GameRules>&);

		const unsigned int& Life() const;
	};

	// --------------------------------------------------------------------
	inline IEntity::IEntity(const unsigned int& id, const EntityType& type, const syb::Vec2& pos) :
		m_Position(pos),
		m_EntityId(id),
		m_Type(type)
	{ }

	inline void IEntity::SetPosition(const syb::Vec2& pos)
	{
		m_Position = pos;
	}

	inline const syb::Vec2& IEntity::Position() const
	{
		return m_Position;
	}

	inline const unsigned int& IEntity::Id() const
	{
		return m_EntityId;
	}

	inline const IEntity::EntityType& IEntity::Type() const
	{
		return m_Type;
	}

	inline IEntity::EntityType IEntity::StringToType(const std::string& type)
	{
		return m_ConversionTable[type];
	}

	// --------------------------------------------------------------------
	inline IRuledEntity::IRuledEntity(const std::shared_ptr<GameRules>& rules) :
		m_pRules(rules)
	{ }

	// --------------------------------------------------------------------
	inline Player::Player(
		const unsigned int& id, 
		const EntityType& type,
		const syb::Vec2& position, 
		const PlayerDirection& direction, 
		const std::shared_ptr<GameRules>& rules,
		const unsigned int& player_id) :
		
		IEntity(id, type, position),
		IRuledEntity(rules),
		m_Direction(direction),
		m_PlayerId(player_id)
	{ }

	inline Player::PlayerDirection Player::StringToDirection(const std::string& direction)
	{
		if (direction == "up")
			return PlayerDirection::UP;
		if (direction == "right")
			return PlayerDirection::RIGHT;
		if (direction == "down")
			return PlayerDirection::DOWN;
		if (direction == "left")
			return PlayerDirection::LEFT;

		return PlayerDirection::NONE;
	}

	inline const Player::PlayerDirection& Player::Direction() const
	{
		return m_Direction;
	}

	inline void Player::SetDirection(const PlayerDirection& direction)
	{
		m_Direction = direction;
	}

	inline const unsigned int& Player::PlayerId() const
	{
		return m_PlayerId;
	}

	inline const unsigned int& Player::Speed() const
	{
		return m_pRules->PlayerSpeed();
	}

	// -------------------------------------------------------------------
	inline FixBlock::FixBlock(const unsigned int& id, const EntityType& type, const syb::Vec2& position) :
		IEntity(id, type, position)
	{ }

	// -------------------------------------------------------------------
	inline Bomb::Bomb(const unsigned int& id, const EntityType& type, const syb::Vec2& position, const std::shared_ptr<GameRules>& rules) :
		IEntity(id, type, position),
		IRuledEntity(rules)
	{ }

	inline const unsigned int& Bomb::Life() const
	{
		return m_pRules->BombLife();
	}

	inline const unsigned int& Bomb::Range() const
	{
		return m_pRules->BombRange();
	}
	
	inline const unsigned int& Bomb::Speed() const
	{
		return m_pRules->BombSpeed();
	}

	// -------------------------------------------------------------------
	inline Flame::Flame(const unsigned int& id, const EntityType& type, const syb::Vec2& position, const std::shared_ptr<GameRules>& rules) :
		IEntity(id, type, position),
		IRuledEntity(rules)
	{ }

	inline const unsigned int& Flame::Life() const
	{
		return m_pRules->FlameLife();
	}
} // namespace boom

#endif // BOOM_ENTITY_HPP