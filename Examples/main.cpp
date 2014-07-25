// Assumes that bomberman.lib and bomberman-d.lib are located in (this project's directory)/lib/
// MUST BE DEFINED BEFORE ANYTHING ELSE if it is to be used
//#define IMPORT_LIBS_FOR_ME
#include "../Bomberman/include/Bomberman.hpp"


class MyBot : public IBot
{
public:
	MyBot(std::string name) : IBot(name) { }
	void Update()
	{
		static unsigned int xx = 1, yy = 1;
		if (players.size())
		{
			xx = players[0].x;
			yy = players[0].y;
			//std::cout << xx << ":" << yy;
		}

		if (players_within_range.size())
			WorldInterface::PlantBomb();
		else if (players.size())
			WorldInterface::GoTo(xx, yy);
	}
};


int main()
{
	BombermanGame game;
	game.ConsoleLog(true, true);

	IBot* myVeryOwnBot = new MyBot("hug4");
	game.RegisterBot(myVeryOwnBot);

	// Register some other bots to run alongside "Bada$$". Theoretically, that is the case.
	// Currently, any bot registered after the first will invalidate the first and replace it. 

	game.Run();
}