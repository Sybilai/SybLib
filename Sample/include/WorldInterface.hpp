#ifndef BOOM_INTERFACE_HPP
#define BOOM_INTERFACE_HPP

#include "sybil/ai/WorldInterface.hpp"
#include "sybil/utils/Time.hpp"
#include "Game.hpp"
#include "World.hpp"
#include "IBot.hpp"
#include <vector>
#include <string>
#include <memory>


namespace boom
{
	class WorldInterface : public syb::WorldInterface
	{
		friend class BombermanGame;
	public:
		static void GoTo(const unsigned int& x, const unsigned int& y);
		static void PlantBomb();

	private:
		WorldInterface();
		static void Init(BombermanGame&);
		static void UpdateBot(const bool& update_map = false);

		static void QueryMap();
		static void QueryBombs();
		static void QueryPlayers();

		static std::string GetDelimiters(const std::string& msg);
		
		static IBot* m_pBot;
		static std::shared_ptr<Player> m_pBotEntity;
		static World* m_pWorld;

		struct Target
		{
			Target();
			unsigned int x;
			unsigned int y;
		} static m_CurrentTarget;

		static syb::Time::Clock m_BombPlantTimeout;
	};
}

#endif // BOOM_INTERFACE_HPP