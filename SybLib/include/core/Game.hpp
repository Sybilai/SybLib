#ifndef SYB_GAME_HPP
#define SYB_GAME_HPP

#include <chrono>
#include <ctime>

#include "networking/IOChannel.hpp"

namespace syb
{
	class Game
	{
	public:
		// ----------------------------------------------------------------------------
		// Game instance initialisation arguments
		// ----------------------------------------------------------------------------
		enum InitArgs
		{
			IARG_CONNECTION_TARGET,
			IARG_DATA_TRANSFER_MODE,
			IARG_TIMESTEP_MODE,
			IARG_BOT_NAME
		};

		// ----------------------------------------------------------------------------
		// Timestep modes
		// ----------------------------------------------------------------------------
		enum TimestepMode
		{
			TIMESTEP_FIXED,
			TIMESTEP_VARIABLE
		};
		const int TS_DEF_FPS = 30;

		// ----------------------------------------------------------------------------
		// Game states
		// ----------------------------------------------------------------------------
		enum GameState
		{
			G_STATE_INIT,
			G_STATE_RUNNING,
			G_STATE_PAUSED
		};

	public:
		virtual ~Game() 
		{
			//delete m_pWorld;
			delete m_pIO;
		}

		virtual int Run()
		{
			while (m_State == G_STATE_INIT)
			{
				HandleEvents();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}

			if (m_TimestepMode == TIMESTEP_FIXED)
			{
				while (m_State)
				{
					if (m_State == G_STATE_PAUSED)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					else
					{
						std::clock_t tickStart = std::clock();

						Update();

						std::clock_t tickEnd = std::clock();
						std::chrono::milliseconds frameSkip(1000 / m_FixedFPS - (tickEnd - tickStart));
						if (frameSkip.count() > 0)
							std::this_thread::sleep_for(frameSkip);
					}
				}
			}
			else if (m_TimestepMode == TIMESTEP_VARIABLE)
			{
				std::clock_t tickStart = std::clock();
				double lag = 0.0;
				while (m_State == G_STATE_RUNNING)
				{
					std::clock_t tickEnd = std::clock();
					std::clock_t elapsed = tickEnd - tickStart;
					tickStart = tickEnd;
					lag += elapsed;

					while (lag >= m_FixedFPS)
					{
						//Update(elapsed);
						lag -= m_FixedFPS;
					}

					tickStart = tickEnd;
				}
			}
			return 0;
		}

	protected:
		Game(IBot *bot) :
			m_pBot(bot)
		{
			m_State = G_STATE_INIT;
			m_TimestepMode = TIMESTEP_FIXED;
			m_FixedFPS = TS_DEF_FPS;
			IOChannel::TcpConnectionParams cParams;
			cParams.address = "sybilai.com";
			cParams.port = "8124";
			m_pIO = new IOChannel(bot->GetName().c_str(), IOChannel::SYB_VIS, &cParams);
		}

		virtual void Update()
		{
			HandleEvents();
			m_pBot->Update();
		}

		//virtual void Update(double dt);

		virtual void HandleEvents() = 0;
		//virtual void CheckTriggers() = 0;

		TimestepMode m_TimestepMode;
		unsigned int m_FixedFPS;
		GameState m_State;
		IOChannel *m_pIO;
		IBot *m_pBot;
		BombWorld *m_pWorld;
	};
} // namespace syb

#endif // SYB_GAME_HPP