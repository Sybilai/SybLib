#ifndef BOOM_BOT_HPP
#define BOOM_BOT_HPP

#include "sybil/ai/IBot.hpp"
#include <string>
#include <vector>


namespace boom
{
	/// Derived bots must call IBot's constructor with some desired name:
	/// class MyCoolTerminator : public IBot
	/// {
	///		MyCoolTerminator(std::string name) : IBot(name) { }
	///     MyCoolTerminator() : IBot("mycoolname") { } 
	///     MyCoolTerminator(someargs) : IBot("string name") { } 
	/// }
	class IBot : public syb::IBot
	{
		/// The world interface may update the bot, without the bot specifically requesting/querying such updates
		friend class WorldInterface;
	public:
		IBot(std::string name);
		~IBot();
		virtual void Update() = 0;

	protected:
		/// Tile the bot is in(at?)
		unsigned int x, y;

		// --------------------------------------------------------------------
		/// Data representations relevant to bots. Don't necessarily match the interal representations.
		// --------------------------------------------------------------------

		/// Avoid scoping
		enum
		{
			WALKABLE = 0,
			EMPTY = 0,
			FIXLBOCK = 1,
			FLAME = 2
		};

		struct Player
		{
			Player(const unsigned int& x_, const unsigned int& y_);
			Player(const float& x_, const float& y_);
			unsigned int x, y;
		};
		
		struct Bomb
		{
			Bomb(const unsigned int& x_, const unsigned int& y_);
			Bomb(const float& x_, const float& y_);
			unsigned int x, y;
		};

		// --------------------------------------------------------------------
		/// Store an image of the world that would be relevant to a bot.
		// --------------------------------------------------------------------
		std::vector<std::vector<unsigned int>> world;

		std::vector<Bomb  > bombs;
		std::vector<Player> players;
		std::vector<Bomb  > bombs_within_range;
		std::vector<Player> players_within_range;

	private:
		unsigned int m_EntityId;
	};

	inline IBot::IBot(std::string name) : syb::IBot(name)
	{ /*this->pWorld = game.m_Interface;*/ }

	inline IBot::~IBot()
	{ }

	inline IBot::Player::Player(const unsigned int& x_, const unsigned int& y_) :
		x(x_), y(y_)
	{ }

	inline IBot::Player::Player(const float& x_, const float& y_) :
		x((unsigned int)x_), y((unsigned int)y_)
	{ }

	inline IBot::Bomb::Bomb(const unsigned int& x_, const unsigned int& y_) :
		x(x_), y(y_)
	{ }

	inline IBot::Bomb::Bomb(const float& x_, const float& y_) :
		x((unsigned int)x_), y((unsigned int)y_)
	{ }
} // namespace boom

#endif // BOOM_BOT_HPP