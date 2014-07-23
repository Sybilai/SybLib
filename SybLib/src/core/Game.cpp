#include "../../include/core/Game.hpp"
#include "../../include/utils/Time.hpp"

#include <thread>
#include <future>
#include <vector>


namespace syb
{
	// --------------------------------------------------------------------
	Game::~Game()
	{ }

	// --------------------------------------------------------------------
	int Game::Run()
	{
		Init();

#pragma region WaitForRelevantDataFromServer

		while (!m_RuleBuffer.ready_for_lunch)
		{
			HandleEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Seems redundant. Don't really remember why this thing was supposed to exist in the first place.
		// TODO: Consider hacking and slashing some if that reasoning doesn't come to mind.
		m_EngineRules.m_UpdateRate = m_RuleBuffer.update_rate;
		m_EngineRules.m_StartFrame = m_RuleBuffer.start_frame;

		m_Frame = m_EngineRules.StartFrame();
		m_FrameTime = 1000000 / m_EngineRules.UpdateRate();

#pragma endregion

#pragma region MainLoop

		while (2 + 2 != 5)
		{
			auto frame_start = Time::GetTime();

			// Do some magic
			Update();
			// Then spread your magic to the syb::World

			unsigned int elapsed_time = Time::FromSecTo(Time::Elapsed(frame_start), Time::Precision::MICROSEC);

			unsigned int elapsed_frames = elapsed_time / m_FrameTime + 1;
			if (elapsed_time > m_DebugTimeThreshold)
				elapsed_frames = 1;

			// Count the actual number of elapsed frames + 1 for the wait time 
			m_Frame += elapsed_frames;

			std::this_thread::sleep_for(std::chrono::microseconds(elapsed_frames * m_FrameTime - elapsed_time));
			// branch prediction, yo! much cpu friendly! very optimise!
			// std::chrono::microseconds frame_skip((int)m_msFrameTime - int_ms);
			// if (frame_skip.count() > 0)
			//	std::this_thread::sleep_for(frame_skip);
		}

#pragma endregion
	}

	// --------------------------------------------------------------------
	const bool& Game::IsInitialised() const
	{
		return m_bEngineInitialised;
	}

	// --------------------------------------------------------------------
	void Game::RegisterWorld(World* pWorld)
	{
		m_pWorld = pWorld;
	}

	// --------------------------------------------------------------------
	void Game::RegisterBot(IBot* bot)
	{
		// Unlikely that the player deletes his bot instance before calling Run()
		//bot->AttachWorldInterface(m_pWorld->Interface());
		// Attach after receiving the game_rules, instead, 
		// i.e. after user calls Run(), which calls Init(), which initialises m_pWorld

		// m_pBots.push_back(bot);
		if (m_pBots.size())
		{
			// Don't delete the bot already in to avoid snafu in the player's own code.
			m_pBots[0] = bot;
		}
		else
			m_pBots.push_back(bot);
	}

	// --------------------------------------------------------------------
	void Game::Update()
	{
		HandleEvents();

		for (auto bot : m_pBots)
			bot->Update();
	}

	// --------------------------------------------------------------------
	void Game::EngineInit()
	{
		// Sonic's my name
		// Speed's my game
		std::future<void> time = std::async(Time::Init);
		//std::future<EventManager*> event_manager = std::async(g_pEventManager->Init());
		//g_pEventManager->Init();
		m_IOManager.Init(m_pBots[0]->Name());

		// Wait for everything to settle
		time.wait();

		// Time to party hard
		// Not really actually. Figure out how to pull the first message from the authority in here
		m_bEngineInitialised = true;
	}

	// --------------------------------------------------------------------
	void Game::Init()
	{
		EngineInit();
		// REGISTER ANY REQUIRED SYSTEMS LIKE WORLD, OTHERWISE BEAR THE FURY OF THE SNAFU
	}

	// --------------------------------------------------------------------
	Game::Game() :
		m_bEngineInitialised(false),
		m_Frame(0),
		m_FrameTime(1000000 / 30),
		m_DebugTimeThreshold(1000000), // implies less than 1 FPS
		m_pWorld(nullptr)
		//g_pEventManager(nullptr)
	{

	}

	Game::RuleBuffer::RuleBuffer() :
		ready_for_lunch(false),
		start_frame(0),
		update_rate(0)
	{ }
} // namespace syb