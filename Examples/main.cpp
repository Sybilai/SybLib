#include <iostream>

#include "games/bomberman/BombGame.hpp"
#include "ai/IBot.hpp"
#include "ai/AStarSearch.hpp"
//#include "ai/SearchTemp.hpp"

#include <ctime>

using namespace syb;

class Terminator : public IBot
{
public:
	Terminator() : IBot("Mitica") { }

	void Update()
	{
		if (std::clock() % 10 == 0)
		{
			if (m_pPos)
			{
				std::string myPath;
				Vec2 myPos = *m_pPos;
				Vec2 target(3, 3);
				myPath = JsonPath(Search(myPos, target));
				Send("move", &myPath);
			}
		}
	}
};


int main()
{
	IBot *myTerminator = new Terminator();
	BombGame game(myTerminator);
	return game.Run();
}