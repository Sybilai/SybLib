#ifndef SYB_GAME_HPP
#define SYB_GAME_HPP

#include "../events/EventManager.hpp"


namespace syb
{
	class Game
	{
	public:
		virtual ~Game();
		virtual int Run();

		Game();
	protected:

		/// Initialises engine "critical" systems. In case Init() is overriden, this should be called in it before anything else. 
		void EngineInit();

		/// Initialises game-specific systems. 
		virtual void Init();
		
		/// Gets called by the main game loop on each frame.
		virtual void Update();

		/// Current frame. May be used to check synch status with the authority. 
		unsigned int m_Frame;

		/// System-wide constant constraint on updating. 
		/// Defaults to 33333microsec = 33.3ms, but should be overriden in a derived class. 
		/// The precision is so hight to compensate for the restricted frame time constraint. 
		unsigned int m_FrameTime;

		/// If a frame takes more than this, it's likely that there has been some debugging going on, i.e. 'slowly' stepping
		/// through code. In such cases, only one frame should be taken into account to maintain consistency. Code-bloat atm. 
		//int m_DebugTimeThreshold;

		/// g_lobal-ish. In that sense, should be passed around to anyone interested. If EventManager weren't inheritable, it 
		/// would be a 'true' global.  
		EventManager* g_pEventManager;

	private:
		bool m_bEngineInitialised;
	};
} // namespace syb

#endif // SYB_GAME_HPP