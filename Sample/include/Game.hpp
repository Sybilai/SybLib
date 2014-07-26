#ifndef BOOM_GAME_HPP
#define BOOM_GAME_HPP

#include "../include/sybil/rapidjson/document.h"
#include "../include/World.hpp"
#include "sybil/core/Game.hpp"


namespace boom
{
	class BombermanGame : public syb::Game
	{
		friend class WorldInterface;
		class IOManager;
	public:
		BombermanGame(const std::string& token);
		BombermanGame(const std::string& connection_target, const std::string& token);
		BombermanGame(const std::string& address, const std::string& port, const std::string& token);
		~BombermanGame();
		
		World m_World;

	protected:
		void Init();
		void HandleEvents();
		void Update();
		syb::IOManager* GetIOManager();

	private:
		/// Event handlers
		/// Consider placing anything event related in a separate class(i.e. the old EventManager)
		void HandleELogin(const rapidjson::Document&);
		void HandleEGameRules(const rapidjson::Document&);
		void HandleEFrame(const rapidjson::Document&);
		void HandleEState(const rapidjson::Document&);
		void HandleEGameOver(const rapidjson::Document&);
	};
} // namespace boom

#endif // BOMB_GAME_HPP