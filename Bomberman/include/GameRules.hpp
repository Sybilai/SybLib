#ifndef BOOM_RULES_HPP
#define BOOM_RULES_HPP


namespace boom
{
	/// Avoid hard-coded rules on the client side. The authority may pick and choose and set all sorts of crazy settings.
	/// And that's okay.
	class GameRules
	{
		friend class BombermanGame;

	public:
		const unsigned int& MapWidth() const;
		const unsigned int& MapHeight() const;

		const unsigned int& BombLife() const;
		const unsigned int& BombRange() const;
		const unsigned int& BombSpeed() const;

		const unsigned int& FlameLife() const;

		const unsigned int& PlayerSpeed() const;

	private:
		// Map layout related
		unsigned int m_MapWidth;
		unsigned int m_MapHeight;

		// Bomb related
		unsigned int m_BombLife;
		unsigned int m_BombRange;
		unsigned int m_BombSpeed;

		// Flame related
		unsigned int m_FlameLife;

		// Player related
		unsigned int m_PlayerSpeed;
	};

#pragma region Accessors
	// Map layout related
	inline const unsigned int& GameRules::MapWidth() const
	{
		return m_MapWidth;
	}
	
	inline const unsigned int& GameRules::MapHeight() const
	{
		return m_MapHeight;
	}

	// Bomb related
	inline const unsigned int& GameRules::BombLife() const
	{
		return m_BombLife;
	}
	
	inline const unsigned int& GameRules::BombRange() const
	{
		return m_BombRange;
	}
	
	inline const unsigned int& GameRules::BombSpeed() const
	{
		return m_BombSpeed;
	}

	// Flame related
	inline const unsigned int& GameRules::FlameLife() const
	{
		return m_FlameLife;
	}

	// Player related
	inline const unsigned int& GameRules::PlayerSpeed() const
	{
		return m_PlayerSpeed;
	}
#pragma endregion
} // namespace boom

#endif // BOOM_RULES_HPP