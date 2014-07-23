#ifndef SYB_GAME_HPP
#define SYB_GAME_HPP

#include "../networking/IOManager.hpp"
#include "../ai/IBot.hpp"
#include "EngineRules.hpp"
#include "World.hpp"
#include <vector>


namespace syb
{
	class Game
	{
	public:
		virtual ~Game();
		virtual int Run();

		const bool& IsInitialised() const;

		/// The "flow" is supposed to look like this:
		/// Real-dude writes his terminator in any approved language without any knowledge of SybLib, and plugs it in SybLab.
		/// SybLab picks up said "script", does some magic, spits out bytecode, which in turn is processed along other "scritps"
		/// Right now, it doesn't look like this at all. 
		/// Instead of force feeding the player's bot into the Game's constructor or Run() function, try to emulate-ish
		/// the desired behavior(described hitherto), allowing for easier modification later into said behavior.
		void RegisterBot(IBot*);

	protected:
		// Only thine offspring shall either touch or see thine private parts.
		Game();

		/// Initialises engine "critical" systems. In case Init() is overriden, this should be called in it before anything else. 
		void EngineInit();

		void RegisterWorld(World*);

		/// Initialises game-specific systems. 
		virtual void Init();

		/// Gets called by the main game loop on each frame.
		virtual void Update();

		virtual void HandleEvents() = 0;

		/// Current frame. May be used to check synch status with the authority. 
		unsigned int m_Frame;

		/// System-wide constant constraint on updating. 
		/// Defaults to 33333microsec = 33.3ms, but should be overriden in a derived class. 
		/// The precision is so hight to compensate for the restricted frame time constraint. 
		unsigned int m_FrameTime;

		/// If a frame takes more than this, it's likely that there has been some debugging going on, i.e. 'slowly' stepping
		/// through code. In such cases, only one frame should be taken into account to maintain consistency.
		unsigned int m_DebugTimeThreshold;

		/// g_lobal-ish. In that sense, should be passed around to anyone interested. If EventManager weren't inheritable, it 
		/// would be a 'true' global.  
		//EventManager* g_pEventManager;

		IOManager m_IOManager;

		World *m_pWorld;

		/// Currently allows only one bot. Subsequent calls to RegisterBot will replace any bot already in m_pBots
		std::vector<IBot*> m_pBots;

		EngineRules m_EngineRules;
		/// Any derived game will fill this
		struct RuleBuffer
		{
			RuleBuffer();
			bool ready_for_lunch; // It's like in launch, but without 'a'. Get it? Hue.
								  // Sounds super funny, therefore I won't change it. never. i mean it.
			unsigned int start_frame;
			unsigned int update_rate;
		} m_RuleBuffer;

	private:
		bool m_bEngineInitialised;
	};
} // namespace syb

#endif // SYB_GAME_HPP