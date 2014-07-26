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
		/// Tile the bot most likely is in.
		unsigned int x, y;

		/// Avoid scoping
		enum 
		{
			WALKABLE = 0,
			EMPTY = 0,
			FIXLBOCK = 1
		};

		/// Stores an image of the world, so that querying every frame is not necessary.
		std::vector<std::vector<unsigned int>> world;

		struct Player
		{
			Player(const unsigned int& x_, const unsigned int& y_);
			Player(const float& x_, const float& y_);

			unsigned int x;
			unsigned int y;
		};
		std::vector<Player> players;
		std::vector<Player> players_within_range;

	private:
		unsigned int m_EntityId;
	};

	inline IBot::IBot(std::string name/*, const BombermanGame& game*/) :
		syb::IBot(name)
	{
		//this->pWorld = game.m_Interface;
	}

	inline IBot::~IBot()
	{ }

	inline IBot::Player::Player(const unsigned int& x_, const unsigned int& y_) :
		x(x_),
		y(y_)
	{ }

	inline IBot::Player::Player(const float& x_, const float& y_) :
		x((unsigned int)x_),
		y((unsigned int)y_)
	{ }
} // namespace boom

#endif // BOOM_BOT_HPP