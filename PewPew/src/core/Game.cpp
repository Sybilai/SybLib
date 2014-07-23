#include "../../include/core/Game.hpp"
#include "../../include/utils/Time.hpp"

#include <thread>
#include <future>
#include <vector>

namespace syb
{
	Game::~Game()
	{

	}

	int Game::Run()
	{
		Init();

		while (2 + 2 != 5)
		{
			auto frame_start = Time::GetTime();
	
			Update();

			unsigned int elapsed_time = Time::FromSecTo(Time::Elapsed(frame_start), Time::Precision::MICROSEC);

			//if (elapsed_time > m_DebugTimeThreshold)

			// Count the actual number of elapsed frames + 1 for the wait time 
			unsigned int elapsed_frames = elapsed_time / m_FrameTime + 1;
			m_Frame += elapsed_frames;

			std::this_thread::sleep_for(std::chrono::microseconds(elapsed_frames * m_FrameTime - elapsed_time));
			// branch prediction, yo! much cpu friendly! very optimise!
			// std::chrono::microseconds frame_skip((int)m_msFrameTime - int_ms);
			// if (frame_skip.count() > 0)
			//	std::this_thread::sleep_for(frame_skip);
		}
	}

	void Game::Update()
	{
		// do update stuff
	}

	void Game::EngineInit()
	{
		// Sonic's my name
		// Speed's my game
		std::future<void> time = std::async(Time::Init);
		//std::future<EventManager*> event_manager = std::async(g_pEventManager->Init());
		g_pEventManager->Init();
		

		// Wait for everything to settle
		time.wait();

		// Time to part hard
		m_bEngineInitialised = true;
	}

	void Game::Init()
	{
		EngineInit();
	}

	Game::Game() :
		m_Frame{},
		m_FrameTime(1000000 / 30),
		g_pEventManager(nullptr)
	{
		
	}
} // namespace syb