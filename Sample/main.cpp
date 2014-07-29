// #define IMPORT_LIBS_FOR_ME
#include "Bomberman.hpp"
#include "Bot.h"


int main()
{
	// Option 1: custom address and port
	// @param1: address:port
	// @param2: token
	// ----------------------
	// Option 2: default address and port
	// @param1: token
	// ----------------------
	// Option 3: custom address and port
	// @param1: address
	// @param2: port
	// @param3: token
	BombermanGame game("sybilai.com:8124", "aT4gpwT2xN0JHpkfogUQew");
	
	// @param1: log received messages from authority
	// @param2: log sent messages to authority
	game.ConsoleLog(true, true);

	IBot* bot = new Bot("myBot");
	game.RegisterBot(bot);

	// Start the game loop
	game.Run();
}