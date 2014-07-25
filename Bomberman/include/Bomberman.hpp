#ifndef BOOM_GAME_HPP
#define BOOM_GAME_HPP

#include "../include/sybil/rapidjson/document.h"
#include "../include/World.hpp"
#include "sybil/core/Game.hpp"


#if defined (_MSC_VER) && /*defined(AI) && defined(DEMOCRACY)*/ defined(IMPORT_LIBS_FOR_ME)
	#ifdef _DEBUG
		#pragma comment(lib, "lib/bomberman-d.lib")
		#define D 1
	#else
		#pragma comment(lib, "lib/bomberman.lib")
		#define R 1	
	#endif
#endif


namespace boom
{
	// class Game : public syb::Game
	class BombermanGame : public syb::Game
	{
		friend class WorldInterface;
		class IOManager;
	public:
		static void TestMe()
		{
			#ifdef D
			std::cout<<"debug";
			#elif defined(R)
			std::cout<<"release";
			#endif
		}
		BombermanGame();
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