#include "WorldInterface.hpp"
#include "Game.hpp"
#include "IBot.hpp"

using namespace boom;

#if defined (_MSC_VER) && /*defined(AI) && defined(DEMOCRACY)*/ defined(IMPORT_LIBS_FOR_ME)
	#ifdef _DEBUG
		#pragma comment(lib, "lib/bomberman-d.lib")
	#else
		#pragma comment(lib, "lib/bomberman.lib")
	#endif
#endif