// Assumes that bomberman.lib and bomberman-d.lib are located in (this project's directory)/lib/
// MUST BE DEFINED BEFORE ANYTHING ELSE if it is to be used
//#define IMPORT_LIBS_FOR_ME
#include "../Bomberman/include/WorldInterface.hpp"
#include "../Bomberman/include/Bomberman.hpp"
#include "../Bomberman/include/IBot.hpp"
using namespace boom;


class MyBot : public IBot
{
public:
	MyBot(std::string name) : IBot(name) { }
	void Update()
	{
		if (players.size())
		{
			WorldInterface::GoTo(players[0].x, players[0].y);
		}
	}
};


int main()
{
	BombermanGame game;

	IBot* myVeryOwnBot = new MyBot("Bada$$");
	game.RegisterBot(myVeryOwnBot);

	// Register some other bots to run alongside "Bada$$". Theoretically, that is the case.
	// Currently, any bot registered after the first will invalidate the first and replace it. 

	game.Run();
}